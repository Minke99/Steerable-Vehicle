%This function takes the input data(uint8 array), data_type(uint8) and a
%flag that indicates whther including the two 0xFF sunc bytes or not
function package = compose_package(user_in,data_type,length,test)
    sync1 = uint8(0xFE);
    sync2 = uint8(0xFE);
    start = uint8(0x7E);
    second_last_sign = uint8(0xFE);
    
    %length = uint8(length(user_in))+1;
    length = uint8(length);
    checksum = getCheckSum([data_type,user_in]);
    if test== 1
        package = [sync1,sync2,start,length,data_type,user_in,checksum];
    else
        package = [start,length,data_type,user_in,second_last_sign,checksum];
        %app.TextArea.Value = "in compose";
    end
    
    string = double(package);
    fprintf('%X ',string);
    fprintf('\n');

end