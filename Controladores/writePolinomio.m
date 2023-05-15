function pol = writePolinomio(array,identifier)
    if identifier==0
        syms s;
    else
        s= tf('s');
    end
    pol = 0*s;
    for i = 1:length(array)
        pol = pol + array(i)*s^(length(array)-i);
    end
end