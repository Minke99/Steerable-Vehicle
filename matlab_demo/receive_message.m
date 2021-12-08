function receive_message(device,app)
    device.Timeout = 2;
    receive = readline(device);
    try
        length = strlength(receive);
    catch
        %f = msgbox('Response Timeout', 'Error','error');
        return;
    end    
        
    disp(receive);
    if receive{1}(length) == 'n'
         while device.NumBytesAvailable == 0
         end
         receive = readline(device);
    elseif receive{1}(length) == 'o'
        %obstacle detected
        fprintf("RECEIVED ROBOT ENTERING AVOIDING");
        app.TextArea.Value = 'Avoiding';
        app.ModeSelect = 2;
    elseif receive{1}(length) == 'f'
        %function check fail
        app.feedback = 1;
    elseif receive{1}(length) == 'y'
        %indicate function check success
        app.feedback = 0;
        %obstacle avoided
        %app.ModeSelect = app.CurrentMode;
        fprintf("GUI RECEIVED 'y' FLAG");
        
    end
   
    
    dt_des = 0.1; % 200 ms
    % initialize clock
    t_st = tic;
    % looping
    while toc(t_st) < dt_des
    end
end