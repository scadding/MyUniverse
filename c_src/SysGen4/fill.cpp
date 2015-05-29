void   main()
{
    int     i;
    int     buf[20][28];
    long    l;

    time(&l);
    sRand(l);

    filltri(buf);
    Initialise();
    for(i = 0; i < 5; i++) {
        plottri(buf[i], 28, 1, 7 + 13*i, 0);
        plottri(buf[i+5], 28, 0, 7 + 13*i, 12);
        plottri(buf[i+10], 28, 1, 14 + 13*i, 7);
        plottri(buf[i+15], 28, 0, 14 + 13*i, 19);
    }
    getch();
    closegraph();
    restorecrtmode();
}

int    pm(a)
int    a;
{
    return((Rand()%(2*a)) - a);
}

void   filltri(buf)
int    buf[20][28];
{
    int     tri, hex;

    for(tri = 0; tri < 20; tri++)
        for(hex = 0; hex < 28; hex++) {
            buf[tri][hex] = 0;
        }
    /* do 12s */
    for(tri = 0; tri < 20; tri++) {
        set(buf, tri, 12, 50);
    }
    /* north poles */
    set(buf, 0, 0,
        (buf[0][12] + buf[0][12] + buf[0][12] + buf[0][12] +
         buf[0][12])/5 + pm(8));
    /* south poles */
    set(buf, 15, 0,
        (buf[15][12] + buf[16][12] + buf[17][12] + buf[18][12] +
         buf[19][12])/5 + pm(8));
    /* north corners */
    for(tri = 10; tri < 15; tri++) {
        set(buf, tri, 0,
            (buf[tri - 10][12] + buf[tri - 9][12] +
             buf[tri - 5][12] + buf[tri - 4][12] +
             buf[tri][12])/5 + pm(8));
    }
    /* south corners */
    for(tri = 5; tri < 10; tri++) {
        set(buf, tri, 0,
            (buf[tri + 10][12] + buf[((tri + 4)%5) + 15][12] +
             buf[tri + 5][12] + buf[((tri + 4)%5) + 10][12] +
             buf[tri][12])/5 + pm(8));
    }
    /* do 6&9 of north */
    for(tri = 0; tri < 5; tri++) {
        set(buf, tri, 9,
            (buf[tri][0] + buf[tri][27] + buf[tri][12] +
             buf[(tri + 1)%5][12])/4 + pm(5));
    }
    /* do 6&9 of south */
    for(tri = 15; tri < 20; tri++) {
        set(buf, tri, 9,
            (buf[tri][0] + buf[tri][27] + buf[tri][12] +
             buf[((tri + 1)%5) + 15][12])/4 + pm(5));
    }
    /* do 6&9 of tropic */
    for(tri = 5; tri < 10; tri++) {
        set(buf, tri, 9,
            (buf[tri][0] + buf[tri][27] + buf[tri][12] +
             buf[tri + 5][12])/4 + pm(5));
        set(buf, tri, 6,
            (buf[tri][0] + buf[tri][21] + buf[tri][12] +
             buf[((tri + 4)%5) + 10][12])/4 + pm(5));
    }
    /* do north tropic 24s */
    for(tri = 0; tri < 5; tri++) {
        set(buf, tri, 24,
            (buf[tri][27] + buf[tri][21] + buf[tri][12] +
             buf[tri + 5][12])/4 + pm(5));
    }
    /* do south tropic 24s */
    for(tri = 15; tri < 20; tri++) {
        set(buf, tri, 24,
            (buf[tri][27] + buf[tri][21] + buf[tri][12] +
             buf[tri - 5][12])/4 + pm(5));
    }
    /* do North edges */
    for(tri = 0; tri < 5; tri++) {
        set(buf, tri, 1,
            (buf[tri][0]*2 + buf[tri][6])/3 + pm(3));
        set(buf, tri, 3,
            (buf[tri][0] + buf[tri][6]*2)/3 + pm(3));
        set(buf, tri, 10,
            (buf[tri][6]*2 + buf[tri][21])/3 + pm(3));
        set(buf, tri, 15,
            (buf[tri][6] + buf[tri][21]*2)/3 + pm(3));
        set(buf, tri, 22,
            (buf[tri][21]*2 + buf[tri][24])/3 + pm(3));
        set(buf, tri, 23,
            (buf[tri][21] + buf[tri][24]*2)/3 + pm(3));
        set(buf, tri, 25,
            (buf[tri][24]*2 + buf[tri][27])/3 + pm(3));
        set(buf, tri, 26,
            (buf[tri][24] + buf[tri][27]*2)/3 + pm(3));
    }
    /* do South edges */
    for(tri = 15; tri < 20; tri++) {
        set(buf, tri, 1,
            (buf[tri][0]*2 + buf[tri][6])/3 + pm(3));
        set(buf, tri, 3,
            (buf[tri][0] + buf[tri][6]*2)/3 + pm(3));
        set(buf, tri, 10,
            (buf[tri][6]*2 + buf[tri][21])/3 + pm(3));
        set(buf, tri, 15,
            (buf[tri][6] + buf[tri][21]*2)/3 + pm(3));
        set(buf, tri, 22,
            (buf[tri][21]*2 + buf[tri][24])/3 + pm(3));
        set(buf, tri, 23,
            (buf[tri][21] + buf[tri][24]*2)/3 + pm(3));
        set(buf, tri, 25,
            (buf[tri][24]*2 + buf[tri][27])/3 + pm(3));
        set(buf, tri, 26,
            (buf[tri][24] + buf[tri][27]*2)/3 + pm(3));
    }
    /* do tropic edges */
    for(tri = 5; tri < 10; tri++) {
        set(buf, tri, 1,
            (buf[tri][0]*2 + buf[tri][6])/3 + pm(3));
        set(buf, tri, 3,
            (buf[tri][0] + buf[tri][6]*2)/3 + pm(3));
        set(buf, tri, 10,
            (buf[tri][6]*2 + buf[tri][21])/3 + pm(3));
        set(buf, tri, 15,
            (buf[tri][6] + buf[tri][21]*2)/3 + pm(3));
        set(buf, tri, 2,
            (buf[tri][0]*2 + buf[tri][9])/3 + pm(3));
        set(buf, tri, 5,
            (buf[tri][0] + buf[tri][9]*2)/3 + pm(3));
        set(buf, tri, 14,
            (buf[tri][9]*2 + buf[tri][21])/3 + pm(3));
        set(buf, tri, 20,
            (buf[tri][9] + buf[tri][21]*2)/3 + pm(3));
    }
    /* do rest */
    for(tri = 0; tri < 20; tri++) {
        buf[tri][4] = (buf[tri][0] + buf[tri][6] + buf[tri][9])/3 + pm(4);
        buf[tri][16] = (buf[tri][6] + buf[tri][21] + buf[tri][24])/3 + pm(4);
        buf[tri][19] = (buf[tri][6] + buf[tri][21] + buf[tri][24])/3 + pm(4);

        buf[tri][7] = (buf[tri][6] + buf[tri][4] + buf[tri][12])/3 + pm(2);
        buf[tri][8] = (buf[tri][9] + buf[tri][4] + buf[tri][12])/3 + pm(2);
        buf[tri][11] = (buf[tri][6] + buf[tri][16] + buf[tri][12])/3 + pm(2);
        buf[tri][13] = (buf[tri][9] + buf[tri][16] + buf[tri][12])/3 + pm(2);
        buf[tri][17] = (buf[tri][16] + buf[tri][24] + buf[tri][12])/3 + pm(2);
        buf[tri][18] = (buf[tri][19] + buf[tri][24] + buf[tri][12])/3 + pm(2);
    }
}
