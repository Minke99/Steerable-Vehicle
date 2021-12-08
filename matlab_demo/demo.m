clear;
clc;

%Use serial com port 3 at 9600 braud rate
device = serialport("COM3",9600);
configureTerminator(device,254);

joy = vrjoystick(1);

prompt = ">>>";


while 1
    
   
    %If 'B' on controller is pressed, quit
    if button(joy,2)
        break
    end
    
    
    % round(x/5)*5

    %Read the x and y axis value and map to 0~250
    x = uint8(interp1([-1,1], [0,250], -1*axis(joy,2)));
    y = uint8(interp1([-1,1], [0,250], axis(joy,4)));
    
    x = round(x/25)*25;
    y = round(y/25)*25;
    pair = [x,y];
    %disp(pair);
    
    %Compose data into a package
    %compose_package(data, data_type, with two 0xFF sync bytes or not)
    transmit = compose_package(pair,uint8(0x0A),3, 0);
 
    %writeline(device,transmit);
    %Enter AT command mode
    %transmit = "+++";
    
    i = 1;
    len = 7;
    %len = strlength(transmit);
    while i<=len
        
        write(device,transmit(i),"uint8"); %Send package via Xbee
        i =i+1;
        
        if i==len-1
            dt_des = 0.05; % 4 ms delay
        else
            dt_des = 0.02; % 4 ms delay
        end
        % initialize clock
        t_st = tic;
        % looping
        while toc(t_st) < dt_des
        end
    end
    
    
    receive = readline(device);
    length = strlength(receive);
    %receive = extract_package(receive);
    disp(receive);
    if receive{1}(length) == 'o'
         while device.NumBytesAvailable == 0
         end
         receive = readline(device);
    end
    
    
    dt_des = 0.1; % 200 ms
    % initialize clock
    t_st = tic;
    % looping
    while toc(t_st) < dt_des
    end
    
end