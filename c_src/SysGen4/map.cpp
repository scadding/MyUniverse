extern int     MaxColors;

/* layout
tri:
00    01    02    03    04
05 10 06 11 07 12 08 13 09 14
   15    16    17    18    19
hex:
       00             21 22 23 24 25 26 27
      01 02            15 16 17 18 19 20
    03 04 05            10 11 12 13 14
   06 07 08 09           06 07 08 09
  10 11 12 13 14          03 04 05
 15 16 17 18 19 20         01 02
21 22 23 24 25 26 27        00
*/
int    left[6] = { 1, 3, 6, 10, 15, 21 };
int    right[6] = { 2, 5, 9, 14, 20, 27 };

void set(int buf[20][28], int tri, int hex, int val)
{
    int     i;

    if(hex == 0) {
        switch(tri/5) {
        case 0:
            buf[0][0] = val;
            buf[1][0] = val;
            buf[2][0] = val;
            buf[3][0] = val;
            buf[4][0] = val;
            break;
        case 1:
            buf[tri][0] = val;
            buf[tri + 10][21] = val;
            buf[((tri + 4)%5) + 15][27] = val;
            buf[tri + 5][21] = val;
            buf[((tri + 4)%5) + 10][27] = val;
            break;
        case 2:
            buf[tri][0] = val;
            buf[tri - 10][27] = val;
            buf[((tri + 1)%5) + 0][21] = val;
            buf[tri - 5][27] = val;
            buf[((tri + 1)%5) + 5][21] = val;
            break;
        case 3:
            buf[15][0] = val;
            buf[16][0] = val;
            buf[17][0] = val;
            buf[18][0] = val;
            buf[19][0] = val;
            break;
        }
        return;
    }
    if(hex == 21) {
        switch(tri/5) {
        case 0:
            set(buf, ((tri + 4)%5) + 10, 0, val);
            break;
        case 1:
            set(buf, ((tri + 4)%5) + 10, 0, val);
            break;
        case 2:
            set(buf, tri - 5, 0, val);
            break;
        case 3:
            set(buf, tri - 10, 0, val);
            break;
        }
        return;
    }
    if(hex == 27) {
        switch(tri/5) {
        case 0:
            set(buf, tri + 10, 0, val);
            break;
        case 1:
            set(buf, tri + 5, 0, val);
            break;
        case 2:
        case 3:
            set(buf, ((tri + 1)%5) + 5, 0, val);
            break;
        }
        return;
    }
    if((hex > 21) && (hex < 27)) {
        buf[tri][hex] = val;
        switch(tri/5) {
        case 0:
        case 2:
            buf[tri + 5][hex] = val;
            break;
        case 1:
        case 3:
            buf[tri - 5][hex] = val;
            break;
        }
        return;
    }
    buf[tri][hex] = val;
    for(i = 0; i < 5; i++) {
        if(hex == left[i]) {
            switch(tri/5) {
            case 0:
                buf[((tri+4)%5) + 0][right[i]] = val;
                break;
            case 1:
                buf[((tri+4)%5) + 10][right[4-i]] = val;
                break;
            case 2:
                buf[tri - 5][right[4-i]] = val;
                break;
            case 3:
                buf[((tri+4)%5) + 15][right[i]] = val;
                break;
            }
            return;
        }
    }
    for(i = 0; i < 5; i++) {
        if(hex == right[i]) {
            switch(tri/5) {
            case 0:
                buf[((tri+1)%5) + 0][left[i]] = val;
                break;
            case 1:
                buf[tri+ 5][left[4-i]] = val;
                break;
            case 2:
                buf[((tri+1)%5) + 5][left[4-i]] = val;
                break;
            case 3:
                buf[((tri+1)%5) + 15][left[i]] = val;
                break;
            }
            return;
        }
    }
}

int adj(int t, int h, int buf[6][2])
{
    int     i;

    for(i = 0; i < 6; i++) {
        buf[i][0] = t;
    }
    buf[2][1] = h - 1;
    buf[3][1] = h + 1;
    switch(h) {
    case 0:
        if(t < 5)
            for(i = 0; i < 5; i++) {
                buf[i][0] = i;
                buf[i][1] = 1;
            } else if(t >= 15)
            for(i = 15; i < 20; i++) {
                buf[i - 15][0] = i;
                buf[i - 15][1] = 1;
            } else {
            buf[0][1] = 1;
            buf[1][1] = 2;
            if(t >= 10) {
                buf[2][0] = t - 10;
                buf[2][1] = 26;
                buf[3][0] = t - 10;
                buf[3][1] = 20;
                buf[4][0] = t - 9;
                buf[4][1] = 22;
            } else {
                buf[2][0] = t + 10;
                buf[2][1] = 22;
                buf[3][0] = t + 10;
                buf[3][1] = 15;
                buf[4][0] = ((t+4)%5) + 15;
                buf[4][1] = 26;
            }
        }
        return(5);
    case 1:
        buf[0][1] = 0;
        buf[1][1] = 3;
        buf[2][1] = 4;
        if((t < 5) || (t >= 15)) {
            buf[4][0] = (t/5)*5 + ((t+4)%5);
            buf[5][0] = buf[4][0];
            buf[4][1] = 1;
            buf[5][1] = 4;
        } else {
            if(t < 10) {
                buf[4][0] = ((t + 4)%5) + 10;
            } else {
                buf[4][0] = t - 5;
            }
            buf[5][0] = buf[4][0];
            buf[4][1] = 26;
            buf[5][1] = 19;
        }
        break;
    case 2:
        if(t < 5) {
            return(adj((t + 1)%5, 1, buf));
        } else if(t >= 15) {
            return(adj(((t + 1)%5) + 15, 1, buf));
        } else if(t < 10) {
            return(adj(t + 5, 15, buf));
        } else {
            return(adj(((t + 1)%5) + 5, 15, buf));
        }
    case 3:
        buf[0][1] = 1;
        buf[1][1] = 7;
        buf[2][1] = 6;
        if((t < 5) || (t >= 15)) {
            buf[4][0] = (t/5)*5 + ((t+4)%5);
            buf[5][0] = buf[4][0];
            buf[4][1] = 4;
            buf[5][1] = 8;
        } else {
            if(t < 10) {
                buf[4][0] = ((t + 4)%5) + 10;
            } else {
                buf[4][0] = t - 5;
            }
            buf[5][0] = buf[4][0];
            buf[4][1] = 13;
            buf[5][1] = 19;
        }
        break;
    case 4:
        buf[0][1] = 1;
        buf[1][1] = 2;
        buf[4][1] = 7;
        buf[5][1] = 8;
        break;
    case 5:
        if(t < 5) {
            return(adj((t + 1)%5, 3, buf));
        } else if(t >= 15) {
            return(adj(((t + 1)%5) + 15, 3, buf));
        } else if(t < 10) {
            return(adj(t + 5, 10, buf));
        } else {
            return(adj(((t + 1)%5) + 5, 10, buf));
        }
    case 6:
        buf[0][1] = 3;
        buf[1][1] = 10;
        buf[2][1] = 11;
        if((t < 5) || (t >= 15)) {
            buf[4][0] = (t/5)*5 + ((t+4)%5);
            buf[5][0] = buf[4][0];
            buf[4][1] = 13;
            buf[5][1] = 8;
        } else {
            if(t < 10) {
                buf[4][0] = ((t + 4)%5) + 10;
            } else {
                buf[4][0] = t - 5;
            }
            buf[5][0] = buf[4][0];
            buf[4][1] = 13;
            buf[5][1] = 8;
        }
        break;
    case 7:
        buf[0][1] = 3;
        buf[1][1] = 4;
        buf[4][1] = 11;
        buf[5][1] = 12;
        break;
    case 8:
        buf[0][1] = 4;
        buf[1][1] = 5;
        buf[4][1] = 12;
        buf[5][1] = 13;
        break;
    case 9:
        if(t < 5) {
            return(adj((t + 1)%5, 6, buf));
        } else if(t >= 15) {
            return(adj(((t + 1)%5) + 15, 6, buf));
        } else if(t < 10) {
            return(adj(t + 5, 6, buf));
        } else {
            return(adj(((t + 1)%5) + 5, 6, buf));
        }
    case 10:
        buf[0][1] = 6;
        buf[1][1] = 15;
        buf[2][1] = 16;
        if((t < 5) || (t >= 15)) {
            buf[4][0] = (t/5)*5 + ((t+4)%5);
            buf[5][0] = buf[4][0];
            buf[4][1] = 13;
            buf[5][1] = 19;
        } else {
            if(t < 10) {
                buf[4][0] = ((t + 4)%5) + 10;
            } else {
                buf[4][0] = t - 5;
            }
            buf[5][0] = buf[4][0];
            buf[4][1] = 8;
            buf[5][1] = 4;
        }
        break;
    case 11:
        buf[0][1] = 6;
        buf[1][1] = 7;
        buf[4][1] = 16;
        buf[5][1] = 17;
        break;
    case 12:
        buf[0][1] = 7;
        buf[1][1] = 8;
        buf[4][1] = 17;
        buf[5][1] = 18;
        break;
    case 13:
        buf[0][1] = 8;
        buf[1][1] = 9;
        buf[4][1] = 18;
        buf[5][1] = 19;
        break;
    case 14:
        if(t < 5) {
            return(adj((t + 1)%5, 10, buf));
        } else if(t >= 15) {
            return(adj(((t + 1)%5) + 15, 10, buf));
        } else if(t < 10) {
            return(adj(t + 5, 3, buf));
        } else {
            return(adj(((t + 1)%5) + 5, 3, buf));
        }
    case 15:
        buf[0][1] = 10;
        buf[1][1] = 21;
        buf[2][1] = 22;
        if((t < 5) || (t >= 15)) {
            buf[4][0] = (t/5)*5 + ((t+4)%5);
            buf[5][0] = buf[4][0];
            buf[4][1] = 19;
            buf[5][1] = 26;
        } else {
            if(t < 10) {
                buf[4][0] = ((t + 4)%5) + 10;
            } else {
                buf[4][0] = t - 5;
            }
            buf[5][0] = buf[4][0];
            buf[4][1] = 1;
            buf[5][1] = 4;
        }
        break;
    case 16:
        buf[0][1] = 10;
        buf[1][1] = 11;
        buf[4][1] = 22;
        buf[5][1] = 23;
        break;
    case 17:
        buf[0][1] = 11;
        buf[1][1] = 12;
        buf[4][1] = 23;
        buf[5][1] = 24;
        break;
    case 18:
        buf[0][1] = 12;
        buf[1][1] = 13;
        buf[4][1] = 24;
        buf[5][1] = 25;
        break;
    case 19:
        buf[0][1] = 13;
        buf[1][1] = 14;
        buf[4][1] = 25;
        buf[5][1] = 26;
        break;
    case 20:
        if(t < 5) {
            return(adj((t + 1)%5, 15, buf));
        } else if(t >= 15) {
            return(adj(((t + 1)%5) + 15, 15, buf));
        } else if(t < 10) {
            return(adj(t + 5, 1, buf));
        } else {
            return(adj(((t + 1)%5) + 5, 1, buf));
        }
    case 21:
        if(t < 5) {
            buf[2][0] = (t + 4)%5;
            buf[4][0] = ((t + 4)%5) + 5;
            buf[1][0] = t + 5;
        } else if(t < 10) {
            return(adj(t - 5, h, buf));
        } else if(t < 15) {
            return(adj(t + 5, h, buf));
        } else {
            buf[2][0] = ((t + 4)%5) + 15;
            buf[4][0] = ((t + 4)%5) + 10;
            buf[1][0] = t - 5;
        }
        buf[2][1] = 26;
        buf[0][1] = 15;
        buf[1][1] = 15;
        buf[4][1] = 20;
        return(5);
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
        if(t < 5) {
            buf[4][0] = t + 5;
        } else if(t < 10) {
            return(adj(t - 5, h, buf));
        } else if(t < 15) {
            return(adj(t + 5, h, buf));
        } else {
            buf[4][0] = t - 5;
        }
        buf[5][0] = buf[4][0];
        buf[0][1] = h - 22 + 15;
        buf[1][1] = h - 22 + 16;
        buf[4][1] = buf[0][1];
        buf[5][1] = buf[1][1];
        break;
    case 27:
        if(t < 5) {
            return(adj((t + 1)%5, 21, buf));
        } else if(t >= 15) {
            return(adj(((t + 1)%5) + 15, 21, buf));
        } else if(t < 10) {
            return(adj(t + 5, 0, buf));
        } else {
            return(adj(((t + 1)%5) + 5, 0, buf));
        }
    }
    return(6);
}

int getrow(int t, int h)
{
    int     row;

    for(row = 0; row < 6; row++)
        if(h < left[row]) {
            break;
        }
    if((t < 5) || (t >= 15)) {
        return(10 - row);
    }
    if((row == 0) || (row == 6)) {
        return(4);
    }
    if((row == 1) || (row == 5)) {
        return(3);
    }
    if((row == 2) || (row == 4)) {
        return(2);
    }
    return(1);
}
