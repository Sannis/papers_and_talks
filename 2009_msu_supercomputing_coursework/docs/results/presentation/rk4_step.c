функция шаг_рунге_кутта_4(поле[], dz, f())
{
    k1[] = f(поле[]);
    
    k2[] = f(поле[] + k1[]*dz/2);
    
    k3[] = f(поле[] + k2[]*dz/2);
    
    k4[] = f(поле[] + k3[]*dz);
    
    return поле[] + (k1[] + 2*k2[] + 2*k3[] + k4[])*dz/6;
}