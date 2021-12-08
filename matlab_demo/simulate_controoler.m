function axis = simulate_controoler()
    rng('shuffle','twister');
    a = -1;
    b = 1;
    axis = (b-a).*rand + a;
end