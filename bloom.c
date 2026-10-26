#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    double qa, qb, qc;
    int    qd;
    double qe;
    int    qf;
    double qh, qi;
    int    qj;
    double qk, ql, qm;
    double qn;
    double qs;
} Vrn;

typedef struct { double ax, ay; int kk; double cr, cg, cb; int gl; } Nod;

static HWND    zwnd;
static HDC     zmdc, zfdc;
static HBITMAP zmbm, zfbm;
static HGDIOBJ zobm, zofb;
static HFONT   zfnt;
static HFONT   ztfn;
static int     zw, zh;
static double  zcw, zch;
static Vrn    *zmot;
static int     znm, zrn;
static Nod    *ztg;  static int ztc;
static Nod    *zfl;  static int zfc;
static Nod    *ztp;  static int ztn;
static long    ztk;
static int     znx;  static double zba;
static int     zfx;  static double zfa;
static int     zTn;  static double zTa;
static int     zsp;
static long    zfmt;
static long    zend;
static int     zphase;
static double  zcx, zloy, zlrx, zlry;

static const char *zpool = "abcdefghijkmnpqrstuvwxyz0123456789$+-*/\\|=<>[]{}#@%&?!.,;:^~()_";
static const char *zfoli = "vwYti*+x^;:,.";
static const char *zblos = "@O&%#8o*0QG";

static double zrnd(void){ return (double)rand() / (double)RAND_MAX; }
static int    zcl(double v){ int r=(int)v; if(r<0)r=0; if(r>255)r=255; return r; }

static void zfp(double x, double y, int g, double r, double gg, double b){
    zfl[zfc].ax = x; zfl[zfc].ay = y; zfl[zfc].gl = g;
    zfl[zfc].cr = r; zfl[zfc].cg = gg; zfl[zfc].cb = b; zfl[zfc].kk = 1; zfc++;
}

static void zfv(double x, double y, int g, double r, double gg, double b, int kind){
    zfl[zfc].ax = x; zfl[zfc].ay = y; zfl[zfc].gl = g;
    zfl[zfc].cr = r; zfl[zfc].cg = gg; zfl[zfc].cb = b; zfl[zfc].kk = kind; zfc++;
}

static void zbuild(void){
    double yt = zh * 0.055;
    zlry = zh * 0.150;
    zlrx = zh * 0.096;
    zloy = zh * 0.610;
    zcx  = zw * 0.5;
    double lt = zloy - zlry;
    double knotH = zch * 4.2;
    double knotTop = lt - knotH;
    double rw = zcw * 1.25;

    ztg = (Nod*)malloc(sizeof(Nod) * 200000);
    ztc = 0;

    for(double y = yt; y <= knotTop; y += zch * 0.60){
        for(double x = zcx - rw; x <= zcx + rw; x += zcw * 0.70){
            double u = fabs(x - zcx) / (rw + 1e-6);
            ztg[ztc].ax = x; ztg[ztc].ay = y; ztg[ztc].kk = 0;
            ztg[ztc].cr = 0.72 + 0.40 * (1.0 - u); ztc++;
        }
    }

    int coils = 7;
    double knw = zcw * 2.15;
    for(int c = 0; c <= coils; c++){
        double y = knotTop + knotH * (double)c/(double)coils;
        double hw = knw * (0.86 + 0.16 * sin(3.14159 * (double)c/(double)coils));
        for(double x = zcx - hw; x <= zcx + hw; x += zcw * 0.56){
            ztg[ztc].ax = x; ztg[ztc].ay = y; ztg[ztc].kk = 1;
            ztg[ztc].cr = 1.08; ztc++;
        }
    }

    int ls = 170, bn = 2;
    for(int i = 0; i < ls; i++){
        double s  = 6.2831853 * (double)i / (double)ls;
        double s2 = 6.2831853 * (double)(i+1) / (double)ls;
        double x1 = zcx + zlrx * sin(s)  * sin(s  * 0.5);
        double y1 = zloy - zlry * cos(s);
        double x2 = zcx + zlrx * sin(s2) * sin(s2 * 0.5);
        double y2 = zloy - zlry * cos(s2);
        double tx = x2 - x1, ty = y2 - y1;
        double tl = sqrt(tx*tx + ty*ty); if(tl < 1e-6) tl = 1e-6;
        double nxn = -ty/tl, nyn = tx/tl;
        for(int io = -bn; io <= bn; io++){
            double off = (double)io * zcw * 0.62;
            ztg[ztc].ax = x1 + nxn * off;
            ztg[ztc].ay = y1 + nyn * off;
            ztg[ztc].kk = 2;
            ztg[ztc].cr = 0.74 + 0.40 * (1.0 - fabs((double)io)/(double)bn);
            ztc++;
        }
    }

    zfl = (Nod*)malloc(sizeof(Nod) * 200000);
    zfc = 0;
    double p1 = zrnd()*6.2831, p2 = zrnd()*6.2831;
    double a1 = 0.16 + zrnd()*0.16, a2 = 0.08 + zrnd()*0.12;

    const char *cen = "@O&08QG%";
    const char *pet = "*ooc><^vx\"";
    const char *stm = "||!i";
    const char *lf  = "<>vVwY";

    double fcy = zloy + zlry * 0.10;
    double cs = zcw * 2.5, rs = zch * 2.3;
    for(double cy = fcy - zlry*1.20; cy <= fcy + zlry*1.20; cy += rs){
        for(double cx0 = zcx - zlrx*1.32; cx0 <= zcx + zlrx*1.32; cx0 += cs){
            double jx = cx0 + (zrnd()*2-1)*zcw*1.1;
            double jy = cy  + (zrnd()*2-1)*zch*0.9;
            double nx = (jx - zcx)/zlrx, ny = (jy - fcy)/zlry;
            double nr = sqrt(nx*nx + ny*ny);
            double ang = atan2(ny, nx);
            double oa = 0.98 + a1*sin(ang + p1) + a2*sin(2.0*ang + p2);
            if(oa < 0.70) oa = 0.70; if(oa > 1.50) oa = 1.50;
            if(nr > oa) continue;
            if(zrnd() > 0.82) continue;

            double tp = zrnd();
            double cr, cg, cb, pr, pg, pb;
            if(tp < 0.26){ cr=0.90; cg=0.20; cb=0.40; pr=0.98; pg=0.56; pb=0.70; }
            else if(tp < 0.48){ cr=0.92; cg=0.62; cb=0.12; pr=0.99; pg=0.86; pb=0.46; }
            else if(tp < 0.68){ cr=0.52; cg=0.26; cb=0.80; pr=0.82; pg=0.60; pb=0.95; }
            else if(tp < 0.84){ cr=0.76; cg=0.12; cb=0.20; pr=0.94; pg=0.42; pb=0.44; }
            else { cr=0.92; cg=0.80; cb=0.34; pr=0.95; pg=0.95; pb=0.97; }

            zfp(jx, jy, cen[rand()%8], cr, cg, cb);

            double po[8][2] = { {-1,0},{1,0},{0,-1},{0,1},{-1,-1},{1,-1},{-1,1},{1,1} };
            for(int k = 0; k < 8; k++){
                if(zrnd() > 0.80) continue;
                zfp(jx + po[k][0]*zcw*0.95, jy + po[k][1]*zch*0.85, pet[rand()%10], pr, pg, pb);
            }
            if(zrnd() < 0.35){
                for(int k = 0; k < 4; k++){
                    if(zrnd() > 0.6) continue;
                    double aa = 6.2831*k/4.0;
                    zfp(jx + cos(aa)*zcw*1.9, jy + sin(aa)*zch*1.7, pet[rand()%10], pr, pg, pb);
                }
            }
            if(zrnd() < 0.70){
                int sl = 1 + rand()%3;
                for(int k = 1; k <= sl; k++){
                    double g = 0.50 + zrnd()*0.25;
                    zfp(jx + (zrnd()*2-1)*zcw*0.3, jy + k*zch*0.9, stm[rand()%4], 0.30*g, 0.64*g, 0.32*g);
                }
                if(zrnd() < 0.6){
                    double g = 0.50 + zrnd()*0.30;
                    zfp(jx + (zrnd()<0.5?-1:1)*zcw*1.0, jy + zch*0.9, lf[rand()%6], 0.32*g, 0.74*g, 0.36*g);
                }
            }
        }
    }

    double fs2 = zcw * 1.0, fr2 = zch * 0.95;
    for(double cy = fcy - zlry*1.18; cy <= fcy + zlry*1.18; cy += fr2){
        for(double cx0 = zcx - zlrx*1.28; cx0 <= zcx + zlrx*1.28; cx0 += fs2){
            double nx = (cx0 - zcx)/zlrx, ny = (cy - fcy)/zlry;
            double nr = sqrt(nx*nx + ny*ny);
            double ang = atan2(ny, nx);
            double oa = 0.98 + a1*sin(ang + p1) + a2*sin(2.0*ang + p2);
            if(oa < 0.70) oa = 0.70; if(oa > 1.50) oa = 1.50;
            if(nr > oa) continue;
            if(zrnd() > 0.30) continue;
            double g = 0.45 + zrnd()*0.35;
            zfp(cx0 + (zrnd()*2-1)*zcw*0.3, cy + (zrnd()*2-1)*zch*0.3,
                zfoli[rand() % (int)strlen(zfoli)], 0.30*g, 0.72*g, 0.36*g);
        }
    }

    int nv = 2;
    double vTop = yt + (knotTop - yt) * 0.16;
    const char *vn = "(){}SseCJj%$?/\\~";
    const char *vl = "vVYw<>";
    for(int v = 0; v < nv; v++){
        double vph = zrnd()*6.2831;
        double vfr = 0.015 + zrnd()*0.010;
        double vam = rw * (1.0 + zrnd()*0.5);
        for(double y = knotTop; y >= vTop; y -= zch*0.52){
            double wx = zcx + sin(y*vfr + vph) * vam;
            double g = 0.50 + zrnd()*0.30;
            zfv(wx, y, vn[rand() % (int)strlen(vn)], 0.26*g, 0.72*g, 0.30*g, 2);
            if(zrnd() < 0.42){
                double sd = (sin(y*vfr + vph) > 0) ? 1.0 : -1.0;
                double gg = 0.55 + zrnd()*0.30;
                zfv(wx + sd*zcw*0.9, y, vl[rand()%6], 0.30*gg, 0.78*gg, 0.36*gg, 2);
            }
            if(zrnd() < 0.07){
                double tp = zrnd(), br, bc, bb;
                if(tp < 0.5){ br=0.96; bc=0.60; bb=0.72; } else { br=0.97; bc=0.85; bb=0.48; }
                zfv(wx, y, '*', br, bc, bb, 2);
            }
        }
    }

    for(int i = 0; i < zfc; i++){
        int bi = i;
        for(int j = i+1; j < zfc; j++) if(zfl[j].ay > zfl[bi].ay) bi = j;
        Nod t = zfl[i]; zfl[i] = zfl[bi]; zfl[bi] = t;
    }
}

static void zspawn(Vrn *m, int top){
    m->qa = zrnd() * zw;
    m->qb = top ? -zrnd()*zch*2.0 : (zrnd()*2.0 - 1.0) * zh;
    m->qc = 0.4 + zrnd()*1.1;
    m->qd = zpool[rand() % (int)strlen(zpool)];
    m->qe = 55 + zrnd()*80;
    m->qf = 0; m->qj = 0;
    m->qk = 0.86; m->ql = 0.92; m->qm = 1.0;
    m->qn = zrnd()*6.28; m->qs = 1.0;
}

static void zmktext(void){
    ztp = (Nod*)malloc(sizeof(Nod) * 60000);
    ztn = 0;
    const char *l1 = "don't do it.";
    const char *l2 = "smell the flowers";
    int fh = (int)(zh * 0.078);
    HFONT tf = CreateFontA(fh, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET,
        OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_SWISS, "Segoe UI");
    int tw = zw, th = fh * 3;
    HDC td = CreateCompatibleDC(zmdc);
    HBITMAP tb = CreateCompatibleBitmap(zmdc, tw, th);
    HGDIOBJ ob = SelectObject(td, tb);
    HGDIOBJ of = SelectObject(td, tf);
    PatBlt(td, 0, 0, tw, th, BLACKNESS);
    SetBkMode(td, OPAQUE);
    SetBkColor(td, RGB(0, 0, 0));
    SetTextColor(td, RGB(255, 255, 255));
    SetTextAlign(td, TA_CENTER | TA_TOP);
    TextOutA(td, tw/2, 0, l1, (int)strlen(l1));
    TextOutA(td, tw/2, fh + fh/5, l2, (int)strlen(l2));

    double oy = zh * 0.820;
    double gc = zh * 0.0062; if(gc < 4) gc = 4;
    for(double y = 0; y < th; y += gc){
        for(double x = 0; x < tw; x += gc){
            COLORREF c = GetPixel(td, (int)x, (int)y);
            if(GetGValue(c) > 100){
                ztp[ztn].ax = x; ztp[ztn].ay = oy + y; ztn++;
            }
        }
    }
    for(int i = 0; i < ztn; i++){
        int bi = i;
        for(int j = i+1; j < ztn; j++) if(ztp[j].ax < ztp[bi].ax) bi = j;
        Nod t = ztp[i]; ztp[i] = ztp[bi]; ztp[bi] = t;
    }
    SelectObject(td, of); SelectObject(td, ob);
    DeleteObject(tb); DeleteObject(tf); DeleteDC(td);
}

static void zinit(void){
    zbuild();
    zmktext();
    zrn = (int)((zw/zcw) * (zh/zch) * 0.14);
    int need = (int)(ztc * 1.7) + 400;
    if(zrn < need) zrn = need;
    if(zrn > 9000) zrn = 9000;
    znm = zrn + zfc + ztn;
    zmot = (Vrn*)malloc(sizeof(Vrn) * znm);
    for(int i = 0; i < zrn; i++) zspawn(&zmot[i], 0);
    for(int i = 0; i < zfc; i++){
        Vrn *m = &zmot[zrn + i];
        if(zfl[i].kk == 2){
            m->qa = zfl[i].ax + (zrnd()*2-1)*zcw*0.4;
            m->qb = zfl[i].ay + zch*2.6;
            m->qc = 1.0;
        } else {
            m->qa = zcx + (zrnd()*2-1)*zlrx*0.30;
            m->qb = zloy + zlry*0.55 + zrnd()*zch;
            m->qc = 0;
        }
        m->qd = zfl[i].gl;
        m->qe = 0; m->qf = 2; m->qj = 1;
        m->qh = zfl[i].ax; m->qi = zfl[i].ay;
        m->qk = zfl[i].cr; m->ql = zfl[i].cg; m->qm = zfl[i].cb;
        m->qn = zrnd()*6.28; m->qs = 1.0;
    }
    for(int i = 0; i < ztn; i++){
        Vrn *m = &zmot[zrn + zfc + i];
        m->qa = ztp[i].ax + (zrnd()*2-1)*zcw*0.3;
        m->qb = ztp[i].ay + zch*1.3;
        m->qc = 1.0; m->qd = zpool[rand() % (int)strlen(zpool)];
        m->qe = 0; m->qf = 2; m->qj = 1;
        m->qh = ztp[i].ax; m->qi = ztp[i].ay;
        m->qk = 0.99; m->ql = 0.96; m->qm = 0.86;
        m->qn = zrnd()*6.28; m->qs = 1.0;
    }
    zsp = 1; znx = 0; zba = 0; zfx = 0; zfa = 0;
    zTn = 0; zTa = 0;
    zphase = 0; zfmt = 0; zend = 0;
}

static void zbind(void){
    Nod *g = &ztg[znx];
    int bi = -1; double bd = 1e18;
    for(int i = 0; i < zrn; i++){
        if(zmot[i].qf != 0) continue;
        double dx = zmot[i].qa - g->ax, dy = zmot[i].qb - g->ay;
        double d = dx*dx + dy*dy;
        if(d < bd){ bd = d; bi = i; }
    }
    if(bi < 0) return;
    zmot[bi].qf = 1; zmot[bi].qj = 1;
    zmot[bi].qh = g->ax; zmot[bi].qi = g->ay;
    double sh = (g->cr > 0.01) ? g->cr : 1.0;
    if(sh < 0.5) sh = 0.5; if(sh > 1.12) sh = 1.12;
    zmot[bi].qs = sh;
    znx++;
}

static void zstep(void){
    ztk++;
    long fs = 110;

    if(ztk >= fs && znx < ztc){
        zba += (double)ztc / 150.0;
        int gd = 0;
        while(zba >= 1.0 && znx < ztc && gd < 5){ zbind(); zba -= 1.0; gd++; }
    }
    if(znx >= ztc && zphase == 0){ zphase = 1; zfmt = ztk; zsp = 0; }
    if(zphase == 1 && ztk >= zfmt + 100){ zphase = 2; }
    if(zphase == 2 && zfx < zfc){
        zfa += (double)zfc / 200.0;
        int gd = 0;
        while(zfa >= 1.0 && zfx < zfc && gd < 6){ zmot[zrn + zfx].qe = 1.0; zfx++; zfa -= 1.0; gd++; }
    }
    if(zphase == 2 && zfx >= zfc){ zphase = 3; zend = ztk; }
    if(zphase == 3 && zTn < ztn){
        zTa += (double)ztn / 42.0;
        int gd = 0;
        while(zTa >= 1.0 && zTn < ztn && gd < 30){ zmot[zrn + zfc + zTn].qe = 1.0; zTn++; zTa -= 1.0; gd++; }
    }

    double pls = 1.0 + 0.06 * sin(ztk * 0.045);

    for(int i = 0; i < znm; i++){
        Vrn *m = &zmot[i];
        if(m->qf == 0){
            m->qb += m->qc;
            if(zrnd() < 0.03) m->qd = zpool[rand() % (int)strlen(zpool)];
            if(m->qb > zh + zch){
                if(zsp) zspawn(m, 1);
                else m->qe = 0;
            }
        } else if(m->qf == 1){
            m->qa += (m->qh - m->qa) * 0.085;
            m->qb += (m->qi - m->qb) * 0.085;
            double tg = (198 + 16 * sin(ztk*0.05 + m->qn)) * m->qs;
            if(tg > 248) tg = 248;
            m->qe += (tg - m->qe) * 0.05;
        } else {
            if(m->qe <= 0) continue;
            double ez = (m->qc > 0.5) ? 0.05 : 0.07;
            m->qa += (m->qh - m->qa) * ez;
            m->qb += (m->qi - m->qb) * ez;
            double tg = (205 + 20 * sin(ztk*0.06 + m->qn)) * pls;
            if(tg > 245) tg = 245;
            m->qe += (tg - m->qe) * 0.05;
        }
    }
}

static void zdraw(void){
    BLENDFUNCTION bf; bf.BlendOp = AC_SRC_OVER; bf.BlendFlags = 0;
    bf.SourceConstantAlpha = (zphase >= 4) ? 50 : 38; bf.AlphaFormat = 0;
    AlphaBlend(zmdc, 0, 0, zw, zh, zfdc, 0, 0, 1, 1, bf);

    SetBkMode(zmdc, TRANSPARENT);
    SelectObject(zmdc, zfnt);
    char bc[2]; bc[1] = 0;
    for(int i = 0; i < znm; i++){
        if(i == zrn + zfc) SelectObject(zmdc, ztfn);
        Vrn *m = &zmot[i];
        if(m->qe < 6) continue;
        if(m->qf == 0 && m->qb > zh + zch) continue;
        double l = m->qe;
        SetTextColor(zmdc, RGB(zcl(l*m->qk), zcl(l*m->ql), zcl(l*m->qm)));
        bc[0] = (char)m->qd;
        TextOutA(zmdc, (int)m->qa, (int)m->qb, bc, 1);
    }
}

static LRESULT CALLBACK zproc(HWND h, UINT u, WPARAM w, LPARAM l){
    switch(u){
        case WM_TIMER:
            zstep(); zdraw();
            InvalidateRect(h, NULL, FALSE);
            if(zphase == 3 && ztk >= zend + 170){ zphase = 4; zend = ztk; }
            if(zphase == 4 && ztk >= zend + 52){ DestroyWindow(h); }
            if(ztk > 1700){ DestroyWindow(h); }
            return 0;
        case WM_PAINT: {
            PAINTSTRUCT ps; HDC d = BeginPaint(h, &ps);
            BitBlt(d, 0, 0, zw, zh, zmdc, 0, 0, SRCCOPY);
            EndPaint(h, &ps); return 0;
        }
        case WM_KEYDOWN:
            if(w == VK_ESCAPE){ DestroyWindow(h); return 0; }
            return 0;
        case WM_RBUTTONDOWN:
            DestroyWindow(h); return 0;
        case WM_DESTROY:
            KillTimer(h, 1); PostQuitMessage(0); return 0;
    }
    return DefWindowProc(h, u, w, l);
}

__declspec(dllexport) void CALLBACK Bloom(HWND h, HINSTANCE ii, LPSTR s, int n){
    srand((unsigned)time(NULL) ^ GetTickCount());
    zw = GetSystemMetrics(SM_CXSCREEN);
    zh = GetSystemMetrics(SM_CYSCREEN);

    WNDCLASSA wc; ZeroMemory(&wc, sizeof(wc));
    wc.lpfnWndProc = zproc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszClassName = "zq_bloom_wnd";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClassA(&wc);

    zwnd = CreateWindowExA(WS_EX_TOPMOST, "zq_bloom_wnd", "",
        WS_POPUP | WS_VISIBLE, 0, 0, zw, zh,
        NULL, NULL, wc.hInstance, NULL);

    HDC wd = GetDC(zwnd);
    zfnt = CreateFontA(18, 0, 0, 0, FW_NORMAL, 0, 0, 0,
        DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
        ANTIALIASED_QUALITY, FIXED_PITCH | FF_MODERN, "Consolas");
    int tfs = (int)(zh * 0.0086); if(tfs < 8) tfs = 8;
    ztfn = CreateFontA(tfs, 0, 0, 0, FW_NORMAL, 0, 0, 0,
        DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
        ANTIALIASED_QUALITY, FIXED_PITCH | FF_MODERN, "Consolas");

    zmdc = CreateCompatibleDC(wd);
    zmbm = CreateCompatibleBitmap(wd, zw, zh);
    zobm = SelectObject(zmdc, zmbm);
    PatBlt(zmdc, 0, 0, zw, zh, BLACKNESS);

    zfdc = CreateCompatibleDC(wd);
    zfbm = CreateCompatibleBitmap(wd, 1, 1);
    zofb = SelectObject(zfdc, zfbm);
    PatBlt(zfdc, 0, 0, 1, 1, BLACKNESS);

    HFONT pf = (HFONT)SelectObject(zmdc, zfnt);
    TEXTMETRICA tm; GetTextMetricsA(zmdc, &tm);
    SelectObject(zmdc, pf);
    zch = tm.tmHeight + 3; zcw = tm.tmAveCharWidth; if(zcw < 1) zcw = 9;

    zinit();

    ShowCursor(FALSE);
    SetForegroundWindow(zwnd);
    SetTimer(zwnd, 1, 55, NULL);

    MSG m;
    while(GetMessage(&m, NULL, 0, 0)){
        TranslateMessage(&m);
        DispatchMessage(&m);
    }

    ShowCursor(TRUE);
    SelectObject(zmdc, zobm); DeleteObject(zmbm); DeleteDC(zmdc);
    SelectObject(zfdc, zofb); DeleteObject(zfbm); DeleteDC(zfdc);
    DeleteObject(zfnt); DeleteObject(ztfn); ReleaseDC(zwnd, wd);
    free(zmot); free(ztg); free(zfl); free(ztp);
}
