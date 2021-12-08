%This function verify the checksum
function result = verify_checksum(string, checksum)
    temp_sum = getCheckSum(string);
    if temp_sum == checksum
        result = 1;
    else
        result = 0;
    end

end