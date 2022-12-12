Func* basis = AbsDifBasis::GenerateBasis();

    int l_1[8];
    int l_2[8];
    for (int i = 0; i < 8; i ++)
    {
        l_1[i] = i * (i + 4);
        l_2[i] = i;
    }

    std::cout << basis[0].Eval(l_1, l_2);