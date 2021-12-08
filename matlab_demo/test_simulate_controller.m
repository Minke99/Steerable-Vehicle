clear;
clc;
prompt = ">>>";

while 1
    user_in = input(prompt, 's');
    if user_in =='q'
        break;
    end
    disp(simulate_controoler());
end