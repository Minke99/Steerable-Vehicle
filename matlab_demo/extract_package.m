%This function takes the received data package and extract it

function string = extract_package(receive) 
    test = receive{1}(1);
    if test ~= char(0x7E)
        %disp("invalid package");
        string = "invalid";
        return
    end
    
    length = double(receive{1}(2));
    string = blanks(length);
    i=1;
    while i<=length
        string(i) = receive{1}(2+i);
        i = i+1;
    end
    
end