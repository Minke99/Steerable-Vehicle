%This function takes the data(data_type+data) and calculate it checksum
function checksum = getCheckSum(string)
    decs = double(string);
    decSum = sum(decs);
    checksum = dec2hex(decSum);
    len = strlength(checksum);
    if len>1
        checksum = checksum(len-1:len);
    end
    checksum=uint8(hex2dec('FF')-hex2dec(checksum));


end