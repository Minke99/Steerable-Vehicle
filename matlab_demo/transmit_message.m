
function transmit_message(device,transmit)
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
end