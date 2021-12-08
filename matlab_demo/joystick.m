clear;
clc;

j = vrjoystick(1,'forcefeedback');

c = caps(j);

while 1
    
    p = pov(j,1);
    x = axis(j,1);
    y = axis(j, 2);
    disp(['x:',num2str(x)]);
    disp(['y:',num2str(y)]);
    
    i=0;
    while i<100000000
        i=i+1;
    end
    
end