#include "serial.h"
#pragma code

/*Private Functions*/
//Add item to buffer
void AddQ(Queue PtrQ, char item); 
//Remove and output item from buffer
char DeleteQ(Queue PtrQ);  
//Check if buffer is empty, return 1 if true
int isQEmpty(Queue PtrQ);      
//Check if buffer is full, return 1 if true
int isQFull(Queue PtrQ);        
//Send a single character through SCI
void outputc_usart (char xc);  
//Serial transmit a string using polling method
void tx_str_polling(char*);
// Calculate and outout the checksum of a message
unsigned char calculate_checksum(char *strPtr, char strLength);
// Compare the message's calculated checksum and received checksum
// Return 1 if they are the same
char verify_checksum(char *strPtr, char strLength, unsigned char checksum);
//Extract the received message and push into the data buffer
char extract_package(Queue SerialBufferPtr);
//Check if the received message is emergency stop
char check_data_type (void);


//Global Variables
struct QNode Buffer; //Serial receive buffer
Queue BufferPtr = &Buffer; //Pointer to Buffer

struct extracted_data_buffer Data; //Data buffer
Extracted_Data DataPtr = &Data; //Pointer to Data Buffer

char emergency_stop_flag; // Global Flag that indicates the received command is 
                          //emergency stop
char sync_flag=0;         //Indicate start byte receive, it is synchronized
char cs_coming = 0;       //Indicate the next coming byte is checksum
char correct_mssg[5];     //char array stores correct feedback message
char stop_mssg[5];        //char array stores stop sending feedback message
char fail_mssg[5];        //char array stores connection fails feedback message


//USART setup
void USART_Init(void){
    Buffer.rear = 0; //Initialize front and rear index
    Buffer.front = 0;
    Buffer.overwrite_flag = 0; //Clear overwrite flag
   
    correct_mssg[0] = 0x7E; //Set correct message package
    correct_mssg[1] = 0x01;
    correct_mssg[2] = 'y';
    correct_mssg[3] = 0xFE;
    correct_mssg[4] = 0x00;
    
    stop_mssg[0] = 0x7E; //Set stop sending message package
    stop_mssg[1] = 0x01;
    stop_mssg[2] = 'o';
    stop_mssg[3] = 0xFE;
    stop_mssg[4] = 0x00;
    
    
    
    fail_mssg[0] = 0x7E; // Set system check fail message
    fail_mssg[1] = 0x01;
    fail_mssg[2] = 'f';
    fail_mssg[3] = 0xFE;
    fail_mssg[4] = 0x00;
   
    
    TXSTA = 0b00100100; //: Enable transmit use high speed baud rate  0x24
    RCSTA = 0b10010000; //: Serial port enable and enable receiver 0x90
    SPBRG   = 64; //9600 baud rate Fosc = 10Mhz, Y=16, X=64, High speed has lower
                   // error than low speed at 9600
    
    TRISCbits.RC7 = 1; //Enable RC7/RX pin for input
    TRISCbits.RC6 = 0; //Enable RV6/TX for output
    
    
}

void Interrupt_enable(void){
    INTCONbits.GIEH = 0; //Disable all interrupts
    INTCONbits.GIEL = 0;
    
    RCONbits.IPEN = 1; //Enable priority mode
    
    //PIE1bits.TXIE = 0; //Enable RC interrupt only
    PIE1bits.RCIE = 1;
    
    IPR1bits.RCIP = 1; //Set  RC interrupts as high priority
 
    
    INTCONbits.TMR0IE = 1; //Enable Timer 0 overflow interrupt
    INTCONbits.TMR0IF = 0; //Clear timer 0 overflow flag
    INTCON2bits.TMR0IP = 0; //Set as low priority
    
    INTCONbits.GIEH = 1; //re-enable all interrupts
    INTCONbits.GIEL = 1;
}

void outputc_usart (char xc){
    //Polling method
    while(!PIR1bits.TXIF);
    TXREG = xc;
    
}


void tx_str_polling(char* strPtr){
    //Polling method
    while(*(strPtr)){
        outputc_usart(*strPtr);
        strPtr++;
    }
    
}

void tx_mssg(char select){
    
    if (select == 0)
    {
        tx_str_polling(stop_mssg);
    }
    else if(select == 1)
    {
        tx_str_polling(correct_mssg);
    }
    
}

void rx232Isr(void){
    char temp_char;
    //Store incoming data
    temp_char = RCREG;
   
    //If it is synchronized (Start Byte received)
    if (sync_flag)
    {
        AddQ(BufferPtr, temp_char); //Add received byte into buffer
        if (cs_coming == 0) //If the next coming byte is not checksum
        {   
            if (temp_char == 0xFE) //If this byte is the end sign
            {
                cs_coming = 1; //This means next byte will be checksum
            }
        }
        else if (cs_coming == 1) //If this byte should be checksum
        {
            sync_flag = 0; //Reset synchronized flag 
            cs_coming = 0; //Reset checksum coming flag
            if (check_data_type()) //If this message is emergency stop
            {
                emergency_stop_flag = 1; 
            }
            else
            {
                emergency_stop_flag = 0;
            }
            PIE1bits.RCIE = 0; //Disable RX interrupt
            
        }
    }
    else
    {
        if (temp_char == 0x7E) //Start byte received
        {
            AddQ(BufferPtr, temp_char);
            sync_flag=1; //Now it is synchronized
            
        }
        else
        {
            sync_flag = 0;
        }
    }
    if (RCSTAbits.OERR) //If overwrite occur
    {
        RCSTAbits.CREN = 0; //Clear the flag by dis and re enable receiving
        RCSTAbits.CREN = 1;
    }
    
    
    
}

//*****************************XBee Operation*******************************
char extract_package(Queue SerialBufferPtr) 
{
    char temp_length;
    char temp_type;
    Queue temp_ptr = SerialBufferPtr;
    char i = 0;
    
    
    if (DeleteQ(temp_ptr) != 0x7E) //0x7E is the start sign
    {
        return 0;
    }
    
    temp_length = DeleteQ(temp_ptr); //Extract data length
    if (temp_length >max_data_size)
    {
        return 0;
    }
    //Store length to buffer
    DataPtr->length = temp_length;
    
    //Store data type to buffer
    DataPtr->data_type = DeleteQ(temp_ptr);
    i++;
    
    while (i<temp_length)
    {
        //Store data to buffer
        DataPtr->data[i-1] = DeleteQ(temp_ptr); 
        i++;
    }
    
    if (DeleteQ(temp_ptr) != 0xFE) //0xFE is the second last
    {
        return 0;
    }
    //Store check sum to buffer
    DataPtr->checksum = DeleteQ(temp_ptr);
    
    //Verify whether the message package received is correct
    if (!(verify_checksum(&(DataPtr->data_type), DataPtr->length, DataPtr->checksum)))
    {
        return 0;
    }
    
    //check whether queue is empty
    if( !(isQEmpty(temp_ptr)))
    {
        return 0; 
    }
    return 1;
}

char verify_checksum(char *strPtr, char strLength, unsigned char checksum)
{
    unsigned int temp_sum = 0; 
    //Calculate the checksum
    temp_sum = calculate_checksum(strPtr, strLength);

    if (temp_sum == checksum)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

unsigned char calculate_checksum(char *strPtr, char strLength)
{
    unsigned int temp_sum = 0; 
    char i = 0;
    //Calculate the hash value of data type + data(s))
    while (i<strLength)
    {
        temp_sum += *(strPtr+i);
        i++;
    }
    temp_sum &= 0xFF;
    temp_sum = 0xFF-temp_sum;
    return temp_sum;
}


void compose_package(char *dataPtr, char dataLength,char dataType, Queue DestinationPtr)
{
    unsigned int temp_sum = 0; 
    char i = 0;
    //Buffer first byte is the start sign
    AddQ(DestinationPtr, 0x7E);
    
    //Buffer second byte is the length
    AddQ(DestinationPtr, dataLength);
    
    //Buffer third byte is the data type
    AddQ(DestinationPtr, dataType);
    
    //Buffer now start with the data, use a while loop and AddQ
    while (i< dataLength)
    {
        AddQ(DestinationPtr, *(dataPtr+i));
        i++;
    }
    
    //add the end sign
    AddQ(DestinationPtr, 0xFE);
    //calculate the checksum
    temp_sum = calculate_checksum(dataPtr, dataLength);
    AddQ(DestinationPtr, temp_sum);
    
    
    
}

char cmd_action (void)
{
    //A flag that indicates robot need to change to automod
    char change_to_auto = 0;
    //If the package is correct
    if (extract_package(BufferPtr))
    {
        //If it is motor command
        if (DataPtr->data_type == 0xA)
        {
            mapping(DataPtr->data[0], DataPtr->data[1]);
        }
        //If it is change to auto mode command
        else if (DataPtr->data_type == 0xB)
        {
            //Switch to auto mode
            change_to_auto = 1;
        }
        //if it is emergency stop flag
        else if (DataPtr->data_type == 0xF)
        {
            //Stop the motor
            mapping(125, 125);
            //Because now the robot is already stopped
            //Emergency stop has been executed
            //We can clear the emergency stop flag
            emergency_stop_flag = 0;
        }
        //If it is an Xbee test command
        else if (DataPtr->data_type == 0xC)
        {
            //Xbee test
            if (DataPtr->data[0] != 0x01) //If testing data not correct
            {
                tx_str_polling(fail_mssg); //Incorrect message
                PIE1bits.RCIE = 1;
                return change_to_auto;
            }
        }
    }
    else
    {
        BufferPtr->front =BufferPtr->rear; //Clear buffer
    }
    //Send out continue message
    tx_str_polling(correct_mssg);
    
    //Re-enable RX interrupt
    PIE1bits.RCIE = 1;
    return change_to_auto;
}

char check_data_type (void)
{
    char temp;
    char temp_char;
    //Data type should be the third byte in the buffer
    temp = ((BufferPtr ->front)+3)%MaxSize;
    temp_char = BufferPtr->Data[temp];
    //if the data type is emergency stop
    if (temp_char == 0xF)
    {
        return 1;
    }
    return 0;
}



//******************Queue Operation*********************************
void EmptyQ(void)
{
    BufferPtr->front =BufferPtr->rear; //Clear buffer
}


void AddQ(Queue PtrQ, char item)
{
    if(PtrQ -> overwrite_flag) //If overwritten occurs
    {
        PtrQ->rear =(PtrQ->rear+1)%MaxSize;
        PtrQ->Data[PtrQ->rear]=item;
        PtrQ->front =(PtrQ->rear); //Also move front index by 1. Because front
                                   //is pointing to the previous position of the
                                   //first data. So it's actually the current 
                                   //position of rear index
    }
    else
    {
        if(isQFull(PtrQ))
        {
        PtrQ->overwrite_flag = 1; //Raise the overwritten flag
        PtrQ->rear =(PtrQ->rear+1)%MaxSize; //Now the front and rear start 
                                            //to be the same. 
                                            //Because in the transmitting ISR,
                                            //it would output one data first, 
                                            //then the front and rear will be 
                                            //different. So don't worry about 
                                            //this would make the program think
                                            //the queue is already empty.
        PtrQ->Data[PtrQ->rear]=item;
        return;
        }
    
        //use +1%Max to move the rear as a loop
        PtrQ->rear =(PtrQ->rear+1)%MaxSize; //Move rear first
        PtrQ->Data[PtrQ->rear]=item; //Then store the data
        
    }
}

char DeleteQ(Queue PtrQ)
{
    char temp;  
    //use +1%Max to move the rear as a loop
    PtrQ->front=(PtrQ->front+1)%MaxSize;
    temp = PtrQ->Data[PtrQ->front];
    return temp;
}

int isQEmpty(Queue PtrQ)
{
    if(PtrQ->front ==PtrQ->rear)
    {
        return 1;
    }

    return 0;
}

int isQFull(Queue PtrQ)
{
     //if front is next to rear, the queue is full 
    if((PtrQ->rear+1)%MaxSize == PtrQ->front){
        return 1;
    }
    return 0;
}