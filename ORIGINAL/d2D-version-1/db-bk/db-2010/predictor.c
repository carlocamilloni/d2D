/* d2D - v. 1.0.9
*  (c) Carlo Camilloni
*  Camilloni C., De Simone A., ..., and Vendruscolo M.
*  TITLE
*  JOURNAL */

/* 
 * compile with
 *  gcc  predictor.c -lm -O9 -Wall -Wextra -ansi -pedantic -o pred.x
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI            3.14159265358979323846
#endif

#define MAXLENGTH 1000

enum {
  A, C, D, E, F, G, H, I, K, L, M, N, P, Q, R, S, T, V, W, Y, X, Z, NR
};

enum {
  CA, CB, CO, HN, HA, NH, NSHIFT, HNNH
};

int debug=0;
int nopsipred=0;
int ppii=0;
int shifty=0;
double thread=0.;

int do_alpha(int ashift, double tcs[MAXLENGTH], int nseq[MAXLENGTH], int sql, int ph, char *dbpath)
{
  char n1[25], n2[25], n3[25], n4[25], n5[25], n6[25], *format, str[MAXLENGTH];
  double mean[21];
  double s1[21][21], d3[21][21], d1[21][21], d2[21][21], d4[21][21];
  double as1, ad3, ad1, ad2, ad4;
  int    i, err, count;
  FILE   *m=NULL, *uno, *due, *tre, *qua, *cin, *par;

  switch(ashift)
  {
    case CA: strcpy(n1,"/helix-db/CA-uno.dat"); strcpy(n2,"/helix-db/CA-due.dat"); strcpy(n3,"/helix-db/CA-tre.dat"); strcpy(n4,"/helix-db/CA-quattro.dat"); strcpy(n6,"/helix-db/CA-parm.dat"); strcpy(n5,"/helix-db/CA-cin.dat"); break;
    case CB: strcpy(n1,"/helix-db/CB-uno.dat"); strcpy(n2,"/helix-db/CB-due.dat"); strcpy(n3,"/helix-db/CB-tre.dat"); strcpy(n4,"/helix-db/CB-quattro.dat"); strcpy(n6,"/helix-db/CB-parm.dat"); strcpy(n5,"/helix-db/CB-cin.dat"); break;
    case CO: strcpy(n1,"/helix-db/CO-uno.dat"); strcpy(n2,"/helix-db/CO-due.dat"); strcpy(n3,"/helix-db/CO-tre.dat"); strcpy(n4,"/helix-db/CO-quattro.dat"); strcpy(n6,"/helix-db/CO-parm.dat"); strcpy(n5,"/helix-db/CO-cin.dat"); break;
    case HN: strcpy(n1,"/helix-db/HN-uno.dat"); strcpy(n2,"/helix-db/HN-due.dat"); strcpy(n3,"/helix-db/HN-tre.dat"); strcpy(n4,"/helix-db/HN-quattro.dat"); strcpy(n6,"/helix-db/HN-parm.dat"); strcpy(n5,"/helix-db/HN-cin.dat"); break;
    case HA: strcpy(n1,"/helix-db/HA-uno.dat"); strcpy(n2,"/helix-db/HA-due.dat"); strcpy(n3,"/helix-db/HA-tre.dat"); strcpy(n4,"/helix-db/HA-quattro.dat"); strcpy(n6,"/helix-db/HA-parm.dat"); strcpy(n5,"/helix-db/HA-cin.dat"); break;
    case NH: strcpy(n1,"/helix-db/NH-uno.dat"); strcpy(n2,"/helix-db/NH-due.dat"); strcpy(n3,"/helix-db/NH-tre.dat"); strcpy(n4,"/helix-db/NH-quattro.dat"); strcpy(n6,"/helix-db/NH-parm.dat"); strcpy(n5,"/helix-db/NH-cin.dat"); break;
  }
  sprintf(str, "%s%s", dbpath, n1);
  uno = fopen(str,"r");
  if (uno==NULL) {fprintf(stderr, "%s file not found!\n", str); return 1;}

  sprintf(str, "%s%s", dbpath, n2);
  due = fopen(str,"r");
  if (due==NULL) {fprintf(stderr, "%s file not found!\n", str); return 1;}

  sprintf(str, "%s%s", dbpath, n3);
  tre = fopen(str,"r");
  if (tre==NULL) {fprintf(stderr, "%s file not found!\n", str); return 1;}

  sprintf(str, "%s%s", dbpath, n4);
  qua = fopen(str,"r");
  if (qua==NULL) {fprintf(stderr, "%s file not found!\n", str); return 1;}

  sprintf(str, "%s%s", dbpath, n5);
  cin = fopen(str,"r");
  if (cin==NULL) {fprintf(stderr, "%s file not found!\n", str); return 1;}

  if(ph==0) {
    switch(ashift)
    {
      case CA: sprintf(str,"%s/helix-db/cs-ca-medi.dat",dbpath); break;
      case CB: sprintf(str,"%s/helix-db/cs-cb-medi.dat",dbpath); break;
      case CO: sprintf(str,"%s/helix-db/cs-co-medi.dat",dbpath); break;
      case HN: sprintf(str,"%s/helix-db/cs-hn-medi.dat",dbpath); break;
      case HA: sprintf(str,"%s/helix-db/cs-ha-medi.dat",dbpath); break;
      case NH: sprintf(str,"%s/helix-db/cs-n-medi.dat",dbpath); break;
    }
    m = fopen(str, "r");
    if (m==NULL) {fprintf(stderr, "%s file not found!\n", str); return 1;}
  } else {
    switch(ashift)
    {
      case CA: sprintf(str,"%s/ph3-helix/cs-ca-medi.dat",dbpath); strcpy(n6,"/ph3-helix/CA-parm.dat"); break;
      case CB: sprintf(str,"%s/ph3-helix/cs-cb-medi.dat",dbpath); strcpy(n6,"/ph3-helix/CB-parm.dat"); break;
      case CO: sprintf(str,"%s/ph3-helix/cs-co-medi.dat",dbpath); strcpy(n6,"/ph3-helix/CO-parm.dat"); break;
      case HN: sprintf(str,"%s/ph3-helix/cs-hn-medi.dat",dbpath); strcpy(n6,"/ph3-helix/HN-parm.dat"); break;
      case HA: sprintf(str,"%s/ph3-helix/cs-ha-medi.dat",dbpath); strcpy(n6,"/ph3-helix/HA-parm.dat"); break;
      case NH: sprintf(str,"%s/ph3-helix/cs-n-medi.dat",dbpath); strcpy(n6,"/ph3-helix/NH-parm.dat"); break;
    }
    m = fopen(str, "r");
    if (m==NULL) {fprintf(stderr, "%s file not found!\n", str); return 1;}
  }

  sprintf(str, "%s%s", dbpath, n6);
  par = fopen(str,"r");
  if (par==NULL) {fprintf(stderr, "%s file not found!\n", str); return 1;}

  err = fscanf(uno, "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s");
  err = fscanf(due, "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s");
  err = fscanf(tre, "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s");
  err = fscanf(qua, "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s");
  err = fscanf(cin, "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s");
 
  format = "%*s %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf";
  for(i=0;i<21;i++)
  {
    err = fscanf(m, "%*s %lf %*f", &mean[i]);

    err = fscanf(uno, format, &s1[i][0], &s1[i][1], &s1[i][2], &s1[i][3], &s1[i][4], &s1[i][5], &s1[i][6], &s1[i][7], &s1[i][8], &s1[i][9], &s1[i][10], &s1[i][11], &s1[i][12], &s1[i][13], &s1[i][14], &s1[i][15], &s1[i][16], &s1[i][17], &s1[i][18], &s1[i][19], &s1[i][20]);

    err = fscanf(due, format, &d1[i][0], &d1[i][1], &d1[i][2], &d1[i][3], &d1[i][4], &d1[i][5], &d1[i][6], &d1[i][7], &d1[i][8], &d1[i][9], &d1[i][10], &d1[i][11], &d1[i][12], &d1[i][13], &d1[i][14], &d1[i][15], &d1[i][16], &d1[i][17], &d1[i][18], &d1[i][19], &d1[i][20]);

    err = fscanf(tre, format, &d2[i][0], &d2[i][1], &d2[i][2], &d2[i][3], &d2[i][4], &d2[i][5], &d2[i][6], &d2[i][7], &d2[i][8], &d2[i][9], &d2[i][10], &d2[i][11], &d2[i][12], &d2[i][13], &d2[i][14], &d2[i][15], &d2[i][16], &d2[i][17], &d2[i][18], &d2[i][19], &d2[i][20]);

    err = fscanf(qua, format, &d3[i][0], &d3[i][1], &d3[i][2], &d3[i][3], &d3[i][4], &d3[i][5], &d3[i][6], &d3[i][7], &d3[i][8], &d3[i][9], &d3[i][10], &d3[i][11], &d3[i][12], &d3[i][13], &d3[i][14], &d3[i][15], &d3[i][16], &d3[i][17], &d3[i][18], &d3[i][19], &d3[i][20]);

    err = fscanf(cin, format, &d4[i][0], &d4[i][1], &d4[i][2], &d4[i][3], &d4[i][4], &d4[i][5], &d4[i][6], &d4[i][7], &d4[i][8], &d4[i][9], &d4[i][10], &d4[i][11], &d4[i][12], &d4[i][13], &d4[i][14], &d4[i][15], &d4[i][16], &d4[i][17], &d4[i][18], &d4[i][19], &d4[i][20]);
  }

  fclose(m);
  fclose(uno);
  fclose(due);
  fclose(tre);
  fclose(qua);
  fclose(cin);

  err = fscanf(par, "%lf", &as1);
  err = fscanf(par, "%lf", &ad1);
  err = fscanf(par, "%lf", &ad2);
  err = fscanf(par, "%lf", &ad3);
  err = fscanf(par, "%lf", &ad4);
  
  fclose(par);

  count=0;
  for(i=0;i<sql;i++)
  {
    if(nseq[i]==G&&(ashift==HA||ashift==CB)) continue;
    if(nseq[i]==P&&(ashift==HN||ashift==NH)) continue;

    tcs[i] = mean[nseq[i]]; 

    if(i>0) { tcs[i] += as1*s1[nseq[i-1]][nseq[i]]; } 
    if(i<sql-1) { tcs[i] += ad1*d1[nseq[i+1]][nseq[i]]; } 
    if(i<sql-2) { tcs[i] += ad2*d2[nseq[i+2]][nseq[i]]; } 
    if(i<sql-3) { tcs[i] += ad3*d3[nseq[i+3]][nseq[i]]; } 
    if(i<sql-4) { tcs[i] += ad4*d4[nseq[i+4]][nseq[i]]; }

    if(debug) {fprintf(stdout, "A %i_CS: %i %f\n", ashift, i, tcs[i]); fflush(stdout);}
  }

  return 0;
}

int do_beta(int ashift, double tcs[MAXLENGTH], int nseq[MAXLENGTH], int sql, int ph, char *dbpath)
{
  char   n1[25], n2[25], n3[25], n4[25], n5[25], *format, str[MAXLENGTH];
  double mean[21];
  double s1[21][21], s2[21][21], d1[21][21], d2[21][21];
  double as1, as2, ad1, ad2;
  int    i, err, count;
  FILE   *m=NULL, *uno, *due, *tre, *qua, *par;

  switch(ashift)
  {
    case CA: strcpy(n1,"beta-db/CA-uno.dat"); strcpy(n2,"beta-db/CA-due.dat"); strcpy(n3,"beta-db/CA-tre.dat"); strcpy(n4,"beta-db/CA-quattro.dat"); strcpy(n5,"beta-db/CA-parm.dat"); break;
    case CB: strcpy(n1,"beta-db/CB-uno.dat"); strcpy(n2,"beta-db/CB-due.dat"); strcpy(n3,"beta-db/CB-tre.dat"); strcpy(n4,"beta-db/CB-quattro.dat"); strcpy(n5,"beta-db/CB-parm.dat"); break;
    case CO: strcpy(n1,"beta-db/CO-uno.dat"); strcpy(n2,"beta-db/CO-due.dat"); strcpy(n3,"beta-db/CO-tre.dat"); strcpy(n4,"beta-db/CO-quattro.dat"); strcpy(n5,"beta-db/CO-parm.dat"); break;
    case HN: strcpy(n1,"beta-db/HN-uno.dat"); strcpy(n2,"beta-db/HN-due.dat"); strcpy(n3,"beta-db/HN-tre.dat"); strcpy(n4,"beta-db/HN-quattro.dat"); strcpy(n5,"beta-db/HN-parm.dat"); break;
    case HA: strcpy(n1,"beta-db/HA-uno.dat"); strcpy(n2,"beta-db/HA-due.dat"); strcpy(n3,"beta-db/HA-tre.dat"); strcpy(n4,"beta-db/HA-quattro.dat"); strcpy(n5,"beta-db/HA-parm.dat"); break;
    case NH: strcpy(n1,"beta-db/NH-uno.dat"); strcpy(n2,"beta-db/NH-due.dat"); strcpy(n3,"beta-db/NH-tre.dat"); strcpy(n4,"beta-db/NH-quattro.dat"); strcpy(n5,"beta-db/NH-parm.dat"); break;
  }
  sprintf(str, "%s/%s", dbpath, n1);
  uno = fopen(str,"r");
  if (uno==NULL) {fprintf(stderr, "%s file not found!\n", str); return 1;}

  sprintf(str, "%s/%s", dbpath, n2);
  due = fopen(str,"r");
  if (due==NULL) {fprintf(stderr, "%s file not found!\n", str); return 1;}

  sprintf(str, "%s/%s", dbpath, n3);
  tre = fopen(str,"r");
  if (tre==NULL) {fprintf(stderr, "%s file not found!\n", str); return 1;}

  sprintf(str, "%s/%s", dbpath, n4);
  qua = fopen(str,"r");
  if (qua==NULL) {fprintf(stderr, "%s file not found!\n", str); return 1;}

  if(ph==0) {
    switch(ashift)
    {
      case CA: sprintf(str,"%s/beta-db/cs-ca-medi.dat", dbpath); break;
      case CB: sprintf(str,"%s/beta-db/cs-cb-medi.dat", dbpath); break;
      case CO: sprintf(str,"%s/beta-db/cs-co-medi.dat", dbpath); break;
      case HN: sprintf(str,"%s/beta-db/cs-hn-medi.dat", dbpath); break;
      case HA: sprintf(str,"%s/beta-db/cs-ha-medi.dat", dbpath); break;
      case NH: sprintf(str,"%s/beta-db/cs-n-medi.dat", dbpath); break;
    }
    m = fopen(str, "r");
    if (m==NULL) {fprintf(stderr, "%s file not found!\n", str); return 1;}
  } else {
    switch(ashift)
    {
      case CA: sprintf(str,"%s/ph3-beta/cs-ca-medi.dat", dbpath); strcpy(n5,"ph3-beta/CA-parm.dat"); break;
      case CB: sprintf(str,"%s/ph3-beta/cs-cb-medi.dat",dbpath); strcpy(n5,"ph3-beta/CB-parm.dat"); break;
      case CO: sprintf(str,"%s/ph3-beta/cs-co-medi.dat",dbpath); strcpy(n5,"ph3-beta/CO-parm.dat"); break;
      case HN: sprintf(str,"%s/ph3-beta/cs-hn-medi.dat",dbpath); strcpy(n5,"ph3-beta/HN-parm.dat"); break;
      case HA: sprintf(str,"%s/ph3-beta/cs-ha-medi.dat",dbpath); strcpy(n5,"ph3-beta/HA-parm.dat"); break;
      case NH: sprintf(str,"%s/ph3-beta/cs-n-medi.dat",dbpath); strcpy(n5,"ph3-beta/NH-parm.dat"); break;
    }
    m = fopen(str, "r");
    if (m==NULL) {fprintf(stderr, "%s file not found!\n", str); return 1;}
  }

  sprintf(str, "%s/%s", dbpath, n5);
  par = fopen(str,"r");
  if (par==NULL) {fprintf(stderr, "%s file not found!\n", str); return 1;}

  err = fscanf(uno, "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s");
  err = fscanf(due, "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s");
  err = fscanf(tre, "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s");
  err = fscanf(qua, "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s");
 
  format = "%*s %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf";
  for(i=0;i<21;i++)
  {
    err = fscanf(m, "%*s %lf %*f", &mean[i]);

    err = fscanf(uno, format, &s1[i][0], &s1[i][1], &s1[i][2], &s1[i][3], &s1[i][4], &s1[i][5], &s1[i][6], &s1[i][7], &s1[i][8], &s1[i][9], &s1[i][10], &s1[i][11], &s1[i][12], &s1[i][13], &s1[i][14], &s1[i][15], &s1[i][16], &s1[i][17], &s1[i][18], &s1[i][19], &s1[i][20]);

    err = fscanf(due, format, &s2[i][0], &s2[i][1], &s2[i][2], &s2[i][3], &s2[i][4], &s2[i][5], &s2[i][6], &s2[i][7], &s2[i][8], &s2[i][9], &s2[i][10], &s2[i][11], &s2[i][12], &s2[i][13], &s2[i][14], &s2[i][15], &s2[i][16], &s2[i][17], &s2[i][18], &s2[i][19], &s2[i][20]);

    err = fscanf(tre, format, &d1[i][0], &d1[i][1], &d1[i][2], &d1[i][3], &d1[i][4], &d1[i][5], &d1[i][6], &d1[i][7], &d1[i][8], &d1[i][9], &d1[i][10], &d1[i][11], &d1[i][12], &d1[i][13], &d1[i][14], &d1[i][15], &d1[i][16], &d1[i][17], &d1[i][18], &d1[i][19], &d1[i][20]);

    err = fscanf(qua, format, &d2[i][0], &d2[i][1], &d2[i][2], &d2[i][3], &d2[i][4], &d2[i][5], &d2[i][6], &d2[i][7], &d2[i][8], &d2[i][9], &d2[i][10], &d2[i][11], &d2[i][12], &d2[i][13], &d2[i][14], &d2[i][15], &d2[i][16], &d2[i][17], &d2[i][18], &d2[i][19], &d2[i][20]);
  }

  fclose(m);
  fclose(uno);
  fclose(due);
  fclose(tre);
  fclose(qua);

  err = fscanf(par, "%lf", &as1);
  err = fscanf(par, "%lf", &as2);
  err = fscanf(par, "%lf", &ad1);
  err = fscanf(par, "%lf", &ad2);
  
  fclose(par);

  count=0;
  for(i=0;i<sql;i++)
  {
    if(nseq[i]==G&&(ashift==HA||ashift==CB)) continue;
    if(nseq[i]==P&&(ashift==HN||ashift==NH)) continue;

    tcs[i] = mean[nseq[i]]; 

    if(i>1) { tcs[i] += as2*s2[nseq[i-2]][nseq[i]]; }
    if(i>0) { tcs[i] += as1*s1[nseq[i-1]][nseq[i]]; }
    if(i<sql-1) {tcs[i] += ad1*d1[nseq[i+1]][nseq[i]]; }
    if(i<sql-2) {tcs[i] += ad2*d2[nseq[i+2]][nseq[i]]; }

    if(debug) {fprintf(stdout, "B %i_CS: %i %f\n", ashift, i, tcs[i]); fflush(stdout);}
  }

  return 0;
}

int do_coil(int ashift, double tcs[MAXLENGTH], int nseq[MAXLENGTH], int sql, int ph, char *dbpath)
{
  char n1[25], n2[25], n3[25], n4[25], n5[25], str[MAXLENGTH];
  char   *format;
  double mean[21];
  double s1[21][21], s2[21][21], d1[21][21], d2[21][21];
  double as1, as2, ad1, ad2;
  int  i, err, count;
  FILE *m=NULL, *uno, *due, *tre, *qua, *par;

  switch(ashift)
  {
    case CA: strcpy(n1,"coil-db/CA-uno.dat"); strcpy(n2,"coil-db/CA-due.dat"); strcpy(n3,"coil-db/CA-tre.dat"); strcpy(n4,"coil-db/CA-quattro.dat"); strcpy(n5,"coil-db/CA-parm.dat"); break;
    case CB: strcpy(n1,"coil-db/CB-uno.dat"); strcpy(n2,"coil-db/CB-due.dat"); strcpy(n3,"coil-db/CB-tre.dat"); strcpy(n4,"coil-db/CB-quattro.dat"); strcpy(n5,"coil-db/CB-parm.dat"); break;
    case CO: strcpy(n1,"coil-db/CO-uno.dat"); strcpy(n2,"coil-db/CO-due.dat"); strcpy(n3,"coil-db/CO-tre.dat"); strcpy(n4,"coil-db/CO-quattro.dat"); strcpy(n5,"coil-db/CO-parm.dat"); break;
    case HN: strcpy(n1,"coil-db/HN-uno.dat"); strcpy(n2,"coil-db/HN-due.dat"); strcpy(n3,"coil-db/HN-tre.dat"); strcpy(n4,"coil-db/HN-quattro.dat"); strcpy(n5,"coil-db/HN-parm.dat"); break;
    case HA: strcpy(n1,"coil-db/HA-uno.dat"); strcpy(n2,"coil-db/HA-due.dat"); strcpy(n3,"coil-db/HA-tre.dat"); strcpy(n4,"coil-db/HA-quattro.dat"); strcpy(n5,"coil-db/HA-parm.dat"); break;
    case NH: strcpy(n1,"coil-db/NH-uno.dat"); strcpy(n2,"coil-db/NH-due.dat"); strcpy(n3,"coil-db/NH-tre.dat"); strcpy(n4,"coil-db/NH-quattro.dat"); strcpy(n5,"coil-db/NH-parm.dat"); break;
  }
  sprintf(str, "%s/%s", dbpath, n1);
  uno = fopen(str,"r");
  if (uno==NULL) {fprintf(stderr, "%s file not found!\n", str); return 1;}

  sprintf(str, "%s/%s", dbpath, n2);
  due = fopen(str,"r");
  if (due==NULL) {fprintf(stderr, "%s file not found!\n", str); return 1;}

  sprintf(str, "%s/%s", dbpath, n3);
  tre = fopen(str,"r");
  if (tre==NULL) {fprintf(stderr, "%s file not found!\n", str); return 1;}

  sprintf(str, "%s/%s", dbpath, n4);
  qua = fopen(str,"r");
  if (qua==NULL) {fprintf(stderr, "%s file not found!\n", str); return 1;}

  if(ph==0) {
    switch(ashift)
    {
      case CA: sprintf(str, "%s/coil-db/cs-ca-medi.dat", dbpath); break;
      case CB: sprintf(str, "%s/coil-db/cs-cb-medi.dat", dbpath); break;
      case CO: sprintf(str, "%s/coil-db/cs-co-medi.dat", dbpath); break;
      case HN: sprintf(str, "%s/coil-db/cs-hn-medi.dat", dbpath); break;
      case HA: sprintf(str, "%s/coil-db/cs-ha-medi.dat", dbpath); break;
      case NH: sprintf(str, "%s/coil-db/cs-n-medi.dat", dbpath); break;
    }
    m = fopen(str, "r");
    if (m==NULL) {fprintf(stderr, "%s file not found!\n", str); return 1;}
  } else {
    switch(ashift)
    {
      case CA: sprintf(str, "%s/ph3-coil/cs-ca-medi.dat", dbpath); strcpy(n5,"ph3-coil/CA-parm.dat"); break;
      case CB: sprintf(str, "%s/ph3-coil/cs-cb-medi.dat", dbpath); strcpy(n5,"ph3-coil/CB-parm.dat"); break;
      case CO: sprintf(str, "%s/ph3-coil/cs-co-medi.dat", dbpath); strcpy(n5,"ph3-coil/CO-parm.dat"); break;
      case HN: sprintf(str, "%s/ph3-coil/cs-hn-medi.dat", dbpath); strcpy(n5,"ph3-coil/HN-parm.dat"); break;
      case HA: sprintf(str, "%s/ph3-coil/cs-ha-medi.dat", dbpath); strcpy(n5,"ph3-coil/HA-parm.dat"); break;
      case NH: sprintf(str, "%s/ph3-coil/cs-n-medi.dat", dbpath); strcpy(n5,"ph3-coil/NH-parm.dat"); break;
    }
    m = fopen(str, "r");
    if (m==NULL) {fprintf(stderr, "%s file not found!\n", str); return 1;}
  }

  sprintf(str, "%s/%s", dbpath, n5);
  par = fopen(str,"r");
  if (par==NULL) {fprintf(stderr, "%s file not found!\n", str); return 1;}

  err = fscanf(uno, "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s");
  err = fscanf(due, "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s");
  err = fscanf(tre, "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s");
  err = fscanf(qua, "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s");
 
  format = "%*s %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf";
  for(i=0;i<21;i++)
  {
    err = fscanf(m, "%*s %lf %*f", &mean[i]);

    err = fscanf(uno, format, &s1[i][0], &s1[i][1], &s1[i][2], &s1[i][3], &s1[i][4], &s1[i][5], &s1[i][6], &s1[i][7], &s1[i][8], &s1[i][9], &s1[i][10], &s1[i][11], &s1[i][12], &s1[i][13], &s1[i][14], &s1[i][15], &s1[i][16], &s1[i][17], &s1[i][18], &s1[i][19], &s1[i][20]);

    err = fscanf(due, format, &s2[i][0], &s2[i][1], &s2[i][2], &s2[i][3], &s2[i][4], &s2[i][5], &s2[i][6], &s2[i][7], &s2[i][8], &s2[i][9], &s2[i][10], &s2[i][11], &s2[i][12], &s2[i][13], &s2[i][14], &s2[i][15], &s2[i][16], &s2[i][17], &s2[i][18], &s2[i][19], &s2[i][20]);

    err = fscanf(tre, format, &d1[i][0], &d1[i][1], &d1[i][2], &d1[i][3], &d1[i][4], &d1[i][5], &d1[i][6], &d1[i][7], &d1[i][8], &d1[i][9], &d1[i][10], &d1[i][11], &d1[i][12], &d1[i][13], &d1[i][14], &d1[i][15], &d1[i][16], &d1[i][17], &d1[i][18], &d1[i][19], &d1[i][20]);

    err = fscanf(qua, format, &d2[i][0], &d2[i][1], &d2[i][2], &d2[i][3], &d2[i][4], &d2[i][5], &d2[i][6], &d2[i][7], &d2[i][8], &d2[i][9], &d2[i][10], &d2[i][11], &d2[i][12], &d2[i][13], &d2[i][14], &d2[i][15], &d2[i][16], &d2[i][17], &d2[i][18], &d2[i][19], &d2[i][20]);
  }

  fclose(m);
  fclose(uno);
  fclose(due);
  fclose(tre);
  fclose(qua);

  err = fscanf(par, "%lf", &as1);
  err = fscanf(par, "%lf", &as2);
  err = fscanf(par, "%lf", &ad1);
  err = fscanf(par, "%lf", &ad2);
  
  fclose(par);

  count=0;
  for(i=0;i<sql;i++)
  {
    if(nseq[i]==G&&(ashift==HA||ashift==CB)) continue;
    if(nseq[i]==P&&(ashift==HN||ashift==NH)) continue;

    tcs[i] = mean[nseq[i]]; 

    if(i>1) tcs[i] += as2*s2[nseq[i-2]][nseq[i]];
    if(i>0) tcs[i] += as1*s1[nseq[i-1]][nseq[i]];
    if(i<sql-1) tcs[i] += ad1*d1[nseq[i+1]][nseq[i]];
    if(i<sql-2) tcs[i] += ad2*d2[nseq[i+2]][nseq[i]];

    if(debug) {fprintf(stdout, "C %i_CS: %i %f\n", ashift, i, tcs[i]); fflush(stdout);}
  }

  return 0;
}

int do_ppii(int ashift, double tcs[MAXLENGTH], int nseq[MAXLENGTH], int sql, int ph, char *dbpath)
{
  char   n1[25], n2[25], n3[25], n4[25], n5[25], *format, str[MAXLENGTH];
  double mean[21];
  double s1[21][21], s2[21][21], d1[21][21], d2[21][21];
  double as1, as2, ad1, ad2;
  int    i, err, count;
  FILE   *m=NULL, *uno, *due, *tre, *qua, *par;

  switch(ashift)
  {
    case CA: strcpy(n1,"ppii-db/CA-uno.dat"); strcpy(n2,"ppii-db/CA-due.dat"); strcpy(n3,"ppii-db/CA-tre.dat"); strcpy(n4,"ppii-db/CA-quattro.dat"); strcpy(n5,"ppii-db/CA-parm.dat"); break;
    case CB: strcpy(n1,"ppii-db/CB-uno.dat"); strcpy(n2,"ppii-db/CB-due.dat"); strcpy(n3,"ppii-db/CB-tre.dat"); strcpy(n4,"ppii-db/CB-quattro.dat"); strcpy(n5,"ppii-db/CB-parm.dat"); break;
    case CO: strcpy(n1,"ppii-db/CO-uno.dat"); strcpy(n2,"ppii-db/CO-due.dat"); strcpy(n3,"ppii-db/CO-tre.dat"); strcpy(n4,"ppii-db/CO-quattro.dat"); strcpy(n5,"ppii-db/CO-parm.dat"); break;
    case HN: strcpy(n1,"ppii-db/HN-uno.dat"); strcpy(n2,"ppii-db/HN-due.dat"); strcpy(n3,"ppii-db/HN-tre.dat"); strcpy(n4,"ppii-db/HN-quattro.dat"); strcpy(n5,"ppii-db/HN-parm.dat"); break;
    case HA: strcpy(n1,"ppii-db/HA-uno.dat"); strcpy(n2,"ppii-db/HA-due.dat"); strcpy(n3,"ppii-db/HA-tre.dat"); strcpy(n4,"ppii-db/HA-quattro.dat"); strcpy(n5,"ppii-db/HA-parm.dat"); break;
    case NH: strcpy(n1,"ppii-db/NH-uno.dat"); strcpy(n2,"ppii-db/NH-due.dat"); strcpy(n3,"ppii-db/NH-tre.dat"); strcpy(n4,"ppii-db/NH-quattro.dat"); strcpy(n5,"ppii-db/NH-parm.dat"); break;
  }
  sprintf(str, "%s/%s", dbpath, n1);
  uno = fopen(str,"r");
  if (uno==NULL) {fprintf(stderr, "%s file not found!\n", str); return 1;}

  sprintf(str, "%s/%s", dbpath, n2);
  due = fopen(str,"r");
  if (due==NULL) {fprintf(stderr, "%s file not found!\n", str); return 1;}

  sprintf(str, "%s/%s", dbpath, n3);
  tre = fopen(str,"r");
  if (tre==NULL) {fprintf(stderr, "%s file not found!\n", str); return 1;}

  sprintf(str, "%s/%s", dbpath, n4);
  qua = fopen(str,"r");
  if (qua==NULL) {fprintf(stderr, "%s file not found!\n", str); return 1;}

  if(ph==0) {
    switch(ashift)
    {
      case CA: sprintf(str, "%s/ppii-db/cs-ca-medi.dat", dbpath); break;
      case CB: sprintf(str, "%s/ppii-db/cs-cb-medi.dat",dbpath); break;
      case CO: sprintf(str, "%s/ppii-db/cs-co-medi.dat",dbpath); break;
      case HN: sprintf(str, "%s/ppii-db/cs-hn-medi.dat",dbpath); break;
      case HA: sprintf(str, "%s/ppii-db/cs-ha-medi.dat",dbpath); break;
      case NH: sprintf(str, "%s/ppii-db/cs-n-medi.dat",dbpath); break;
    }
    m = fopen(str, "r");
    if (m==NULL) {fprintf(stderr, "%s file not found!\n", str); return 1;}
  } else {
    switch(ashift)
    {
      case CA: sprintf(str, "%s/ph3-ppii/cs-ca-medi.dat", dbpath); strcpy(n5,"ph3-ppii/CA-parm.dat"); break;
      case CB: sprintf(str, "%s/ph3-ppii/cs-cb-medi.dat", dbpath); strcpy(n5,"ph3-ppii/CB-parm.dat"); break;
      case CO: sprintf(str, "%s/ph3-ppii/cs-co-medi.dat", dbpath); strcpy(n5,"ph3-ppii/CO-parm.dat"); break;
      case HN: sprintf(str, "%s/ph3-ppii/cs-hn-medi.dat", dbpath); strcpy(n5,"ph3-ppii/HN-parm.dat"); break;
      case HA: sprintf(str, "%s/ph3-ppii/cs-ha-medi.dat", dbpath); strcpy(n5,"ph3-ppii/HA-parm.dat"); break;
      case NH: sprintf(str, "%s/ph3-ppii/cs-n-medi.dat", dbpath); strcpy(n5,"ph3-ppii/NH-parm.dat"); break;
    }
    m = fopen(str, "r");
    if (m==NULL) {fprintf(stderr, "%s file not found!\n", str); return 1;}
  }

  sprintf(str, "%s/%s", dbpath, n5);
  par = fopen(str,"r");
  if (par==NULL) {fprintf(stderr, "%s file not found!\n", str); return 1;}

  err = fscanf(uno, "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s");
  err = fscanf(due, "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s");
  err = fscanf(tre, "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s");
  err = fscanf(qua, "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s");
 
  format = "%*s %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf";
  for(i=0;i<21;i++)
  {
    err = fscanf(m, "%*s %lf %*f", &mean[i]);

    err = fscanf(uno, format, &s1[i][0], &s1[i][1], &s1[i][2], &s1[i][3], &s1[i][4], &s1[i][5], &s1[i][6], &s1[i][7], &s1[i][8], &s1[i][9], &s1[i][10], &s1[i][11], &s1[i][12], &s1[i][13], &s1[i][14], &s1[i][15], &s1[i][16], &s1[i][17], &s1[i][18], &s1[i][19], &s1[i][20]);

    err = fscanf(due, format, &s2[i][0], &s2[i][1], &s2[i][2], &s2[i][3], &s2[i][4], &s2[i][5], &s2[i][6], &s2[i][7], &s2[i][8], &s2[i][9], &s2[i][10], &s2[i][11], &s2[i][12], &s2[i][13], &s2[i][14], &s2[i][15], &s2[i][16], &s2[i][17], &s2[i][18], &s2[i][19], &s2[i][20]);

    err = fscanf(tre, format, &d1[i][0], &d1[i][1], &d1[i][2], &d1[i][3], &d1[i][4], &d1[i][5], &d1[i][6], &d1[i][7], &d1[i][8], &d1[i][9], &d1[i][10], &d1[i][11], &d1[i][12], &d1[i][13], &d1[i][14], &d1[i][15], &d1[i][16], &d1[i][17], &d1[i][18], &d1[i][19], &d1[i][20]);

    err = fscanf(qua, format, &d2[i][0], &d2[i][1], &d2[i][2], &d2[i][3], &d2[i][4], &d2[i][5], &d2[i][6], &d2[i][7], &d2[i][8], &d2[i][9], &d2[i][10], &d2[i][11], &d2[i][12], &d2[i][13], &d2[i][14], &d2[i][15], &d2[i][16], &d2[i][17], &d2[i][18], &d2[i][19], &d2[i][20]);
  }

  fclose(m);
  fclose(uno);
  fclose(due);
  fclose(tre);
  fclose(qua);

  err = fscanf(par, "%lf", &as1);
  err = fscanf(par, "%lf", &as2);
  err = fscanf(par, "%lf", &ad1);
  err = fscanf(par, "%lf", &ad2);
  
  fclose(par);

  count=0;
  for(i=0;i<sql;i++)
  {
    if(nseq[i]==G&&(ashift==HA||ashift==CB)) continue;
    if(nseq[i]==P&&(ashift==HN||ashift==NH)) continue;

    tcs[i] = mean[nseq[i]]; 

    if(i>1) { tcs[i] += as2*s2[nseq[i-2]][nseq[i]]; }
    if(i>0) { tcs[i] += as1*s1[nseq[i-1]][nseq[i]]; }
    if(i<sql-1) {tcs[i] += ad1*d1[nseq[i+1]][nseq[i]]; }
    if(i<sql-2) {tcs[i] += ad2*d2[nseq[i+2]][nseq[i]]; }

    if(debug) {fprintf(stdout, "P %i_CS: %i %f\n", ashift, i, tcs[i]); fflush(stdout);}
  }

  return 0;
}

int do_predict(int ashift, char *bmrb, int ph, char *dbpath, char *out, int firstres, char *argv[], int argc)
{
  int    i, j, sql=0, nseq[MAXLENGTH], result, err, MAX;
  int    start, end, by; 
  int    mtot, coeff;
  FILE   *ref, *tab=NULL, *pred;
  double ecs[MAXLENGTH], alphacs[MAXLENGTH], betacs[MAXLENGTH], coilcs[MAXLENGTH], ppiics[MAXLENGTH];
  double sigma[4];
  double Aprob[MAXLENGTH][NSHIFT], Bprob[MAXLENGTH][NSHIFT], Cprob[MAXLENGTH][NSHIFT], Pprob[MAXLENGTH][NSHIFT];
  double pp[MAXLENGTH][4], spp[MAXLENGTH][4], norm, maxi, intp[MAXLENGTH][4], prec;
  double mhelix, mbeta, mcoil, mppii, dcount;
  double ns[6], pcs[MAXLENGTH];
  char   seq[MAXLENGTH], n7[256], *format=NULL, ss[MAXLENGTH], css[MAXLENGTH], str[MAXLENGTH];

/* 11 Dec 2010 */
  ns[5]=1.17;
  ns[4]=1.15;
  ns[3]=1.06;
  ns[2]=0.99;
  ns[1]=1.00;
  ns[0]=1.00;

  if(ph==1) {
/* 11 Dec 2010 */
    ns[5]=0.73;
    ns[4]=0.50;
    ns[3]=0.26;
    ns[2]=0.80;
    ns[1]=1.00;
    ns[0]=1.00;
  }

  if(debug) {fprintf(stdout, "In do predict: ashift is %i bmrb is %s out is %s ph is %i\n", ashift, bmrb, out, ph); fflush(stdout);}

  for(i=0;i<NSHIFT;i++) for(j=0;j<MAXLENGTH;j++) {Aprob[j][i]=999.999; Bprob[j][i]=999.999; Cprob[j][i]=999.999; Pprob[j][i]=999.999;}
  for(j=0;j<MAXLENGTH;j++) {seq[j]=0; css[j]=0; ss[j]=0; }

  if(ashift==NSHIFT) {start=0; end=NH; by=1; strcpy(n7,"SS-results.dat");}
  else if(ashift==HNNH) {start=HN; end=NH; by=2; strcpy(n7,"SS-hn_nh.dat"); if(debug) {fprintf(stderr,"HNNH mode\n"); fflush(stderr);}}
  else {
    start = ashift; 
    end = ashift;
    by = 1;
    switch(ashift)
    {
      case CA: strcpy(n7,"SS-ca.dat"); break;
      case CB: strcpy(n7,"SS-cb.dat"); break;
      case CO: strcpy(n7,"SS-co.dat"); break;
      case HN: strcpy(n7,"SS-hn.dat"); break;
      case HA: strcpy(n7,"SS-ha.dat"); break;
      case NH: strcpy(n7,"SS-nh.dat"); break;
    }
  }

  if(out!=NULL) strcpy(n7,out);
  if(debug) {fprintf(stdout, "In do predict: out is %s n7 is %s \n", out, n7); fflush(stdout);}

  for(ashift=start;ashift<(end+1);ashift=ashift+by)
  {
    if(ph==0) sprintf(str, "%s/other-db/tab.err", dbpath);
    else if(ph==1) sprintf(str, "%s/ph3-other/tab.err", dbpath);

    if(debug) {fprintf(stdout, "In do predict: I'm going to open %s ", str); fflush(stdout);}
    tab = fopen(str,"r");
    if (tab==NULL) {fprintf(stderr, "%s file not found!\n", str); return 1;}

    err = fscanf(tab, "%*s %*s %*s %*s %*s");
    for(i=0;i<NSHIFT;i++)
    {
      if(i==ashift) err = fscanf(tab, "%*s %lf %lf %lf %lf", &sigma[0], &sigma[1], &sigma[2], &sigma[3]);
      else err = fscanf(tab, "%*s %*f %*f %*f %*f");
    }
    fclose(tab);
    if(debug) {fprintf(stdout, "done\n"); fflush(stdout);}

    switch(ashift)
    {
      case CA: 
               format = "%*i %*i %c %*c %c %lf %*lf %*lf %*lf %*lf %*lf"; 
               if(shifty) format = "%*i %c %*lf %lf %*lf %*lf %*lf %*lf"; 
               break;
      case CB: 
               format = "%*i %*i %c %*c %c %*lf %lf %*lf %*lf %*lf %*lf"; 
               if(shifty) format = "%*i %c %*lf %*lf %lf %*lf %*lf %*lf"; 
               break;
      case CO: 
               format = "%*i %*i %c %*c %c %*lf %*lf %lf %*lf %*lf %*lf"; 
               if(shifty) format = "%*i %c %*lf %*lf %*lf %lf %*lf %*lf"; 
               break;
      case HN: 
               format = "%*i %*i %c %*c %c %*lf %*lf %*lf %lf %*lf %*lf"; 
               if(shifty) format = "%*i %c %*lf %*lf %*lf %*lf %*lf %lf"; 
               break;
      case HA: 
               format = "%*i %*i %c %*c %c %*lf %*lf %*lf %*lf %lf %*lf"; 
               if(shifty) format = "%*i %c %lf %*lf %*lf %*lf %*lf %*lf"; 
               break;
      case NH: 
               format = "%*i %*i %c %*c %c %*lf %*lf %*lf %*lf %*lf %lf"; 
               if(shifty) format = "%*i %c %*lf %*lf %*lf %*lf %*lf %lf"; 
               break;
    }

    ref = fopen(bmrb,"r");
    if (ref==NULL) {fprintf(stderr, "%s file not found!\n", bmrb); return 1;}

    sql=0;
    if(shifty) err = fscanf(ref, "%*s %*s %*s %*s %*s %*s %*s %*s");
    while(1)
    {
      if(!shifty) { 
        err = fscanf(ref, format, &seq[sql], &css[sql], &ecs[sql]);
        if(err==EOF) break;
        if(err!=3) {fprintf(stderr, "ERROR: WRONG FILE FORMAT AROUND LINE %i\n", sql+1); exit(1); }
      }
      if(shifty) { err = fscanf(ref, format, &seq[sql], &ecs[sql]); css[sql]='X'; if(err==EOF) break;}
      sql++;
    }
    fclose(ref);
    if(ashift==start) fprintf(stderr,"Protein length is %i. Last residue is %c\n", sql, seq[sql-1]); fflush(stderr);

    for(i=0;i<sql;i++) {
      switch(seq[i]) 
      {
        case 'A': nseq[i] = A; break;
        case 'C': nseq[i] = C; break;
        case 'D': nseq[i] = D; break;
        case 'E': nseq[i] = E; break;
        case 'F': nseq[i] = F; break;
        case 'G': nseq[i] = G; break;
        case 'H': nseq[i] = H; break;
        case 'I': nseq[i] = I; break;
        case 'K': nseq[i] = K; break;
        case 'L': nseq[i] = L; break;
        case 'M': nseq[i] = M; break;
        case 'N': nseq[i] = N; break;
        case 'O': nseq[i] = P; break;
        case 'P': nseq[i] = P; break;
        case 'Q': nseq[i] = Q; break;
        case 'R': nseq[i] = R; break;
        case 'S': nseq[i] = S; break;
        case 'T': nseq[i] = T; break;
        case 'V': nseq[i] = V; break;
        case 'W': nseq[i] = W; break;
        case 'Y': nseq[i] = Y; break;
        case 'X': nseq[i] = X; break;
        case 'Z': nseq[i] = Z; break;
        default: fprintf(stderr, "ERROR: AminoAcids %c not recognized! line %d\n", seq[i], i); return 1; break;
      }
    }

    if(debug) {fprintf(stdout, "In do predict before calling do_alpha\n"); fflush(stdout);}
    result = do_alpha(ashift, alphacs, nseq, sql, ph, dbpath);
    if(result) return result;
    if(debug) {fprintf(stdout, "In do predict before calling do_beta\n"); fflush(stdout);}
    result = do_beta(ashift, betacs, nseq, sql, ph, dbpath);
    if(result) return result;
    if(debug) {fprintf(stdout, "In do predict before calling do_coil\n"); fflush(stdout);}
    result = do_coil(ashift, coilcs, nseq, sql, ph, dbpath);
    if(result) return result;
    if(debug) {fprintf(stdout, "In do predict before calling do_ppii\n"); fflush(stdout);}
    result = do_ppii(ashift, ppiics, nseq, sql, ph, dbpath);
    if(result) return result;

    for(i=0;i<sql;i++) 
    {
      if(ecs[i]<900&&ecs[i]>0.&&(fabs(ecs[i]-alphacs[i])<2.*sigma[0]||fabs(ecs[i]-betacs[i])<2.*sigma[1]||fabs(ecs[i]-coilcs[i])<2.*sigma[2])) {
        Aprob[i][ashift] = 1./(sigma[0]*sqrt(2.*M_PI))*exp(-(ecs[i]-alphacs[i])*(ecs[i]-alphacs[i])/(2.*sigma[0]*sigma[0]));
        Bprob[i][ashift] = 1./(sigma[1]*sqrt(2.*M_PI))*exp(-(ecs[i]-betacs[i])*(ecs[i]-betacs[i])/(2.*sigma[1]*sigma[1]));
        Cprob[i][ashift] = 1./(sigma[2]*sqrt(2.*M_PI))*exp(-(ecs[i]-coilcs[i])*(ecs[i]-coilcs[i])/(2.*sigma[2]*sigma[2]));
        Pprob[i][ashift] = 1./(sigma[3]*sqrt(2.*M_PI))*exp(-(ecs[i]-ppiics[i])*(ecs[i]-ppiics[i])/(2.*sigma[3]*sigma[3]));
      } else {
        Aprob[i][ashift] = 999.999;
        Bprob[i][ashift] = 999.999;
        Cprob[i][ashift] = 999.999;
        Pprob[i][ashift] = 999.999;
      }
      if(debug) {fprintf(stderr, "Scratch PROBs A_%i CS_%i H_%6.3f E_%6.3f C_%6.3f P_%6.3f\n", i, ashift, Aprob[i][ashift], Bprob[i][ashift], Cprob[i][ashift], Pprob[i][ashift]); fflush(stderr); }
    }

  }

  for(i=0;i<sql;i++) 
  {
    coeff=0;
    for(ashift=start;ashift<(end+1);ashift=ashift+by)
    {
      if(Aprob[i][ashift]<900) coeff++;
    }
    switch(coeff)
    {
       case 6: pcs[i]=ns[5]; break;
       case 5: pcs[i]=ns[4]; break;
       case 4: pcs[i]=ns[3]; break;
       case 3: pcs[i]=ns[2]; break;
       case 2: pcs[i]=ns[1]; break;
       case 1: pcs[i]=ns[0]; break;
    } 
  }


  if(!nopsipred) {
    fprintf(stderr,"Executing psipred...");
    tab = fopen("fasta","w");
    for(i=0;i<sql;i++) {
      if(seq[i]!='O'&&seq[i]!='X'&&seq[i]!='Z') fprintf(tab, "%c", seq[i]);
      else if(seq[i]=='O') fprintf(tab, "%c", 'P');
      else if(seq[i]=='X') fprintf(tab, "%c", 'C');
      else if(seq[i]=='Z') fprintf(tab, "%c", ' ');
    }
    fprintf(tab,"\n"); 
    fclose(tab);

    sprintf(str, "%s/psipred/bin/seq2mtx fasta > tmp.mtx 2> /dev/null", dbpath);
    if(debug) {fprintf(stdout, "In do predict I'm going to execute %s\n", str); fflush(stdout);}
    err = system(str);
    if(err!=0) {fprintf(stderr, "PSIPRED (seq2mtx) exit with error %d\n", err); exit(1);}

    sprintf(str, "%s/psipred/bin/psipred tmp.mtx %s/psipred/data/weights_s.dat %s/psipred/data/weights_s.dat2 %s/psipred/data/weights_s.dat3 > psipred.dat 2> /dev/null", dbpath, dbpath, dbpath, dbpath);
    if(debug) {fprintf(stdout, "In do predict I'm going to execute %s\n", str); fflush(stdout);}
    err = system(str);
    if(err!=0) {fprintf(stderr, "PSIPRED (psipred) exit with error %d\n", err); exit(1);}

    tab = fopen("psipred.dat","r");
    if (tab==NULL) {fprintf(stderr, "psipred.dat file not found!\n"); return 1;}

    for(i=0;i<sql;i++) { 
      err = fscanf(tab, "%*d %*s %*s %lf %lf %lf", &intp[i][2], &intp[i][0], &intp[i][1]);
      if(debug) {fprintf(stderr, "PsiPred PROBs A_%i H_%6.3f E_%6.3f C_%6.3f\n", i, intp[i][0], intp[i][1], intp[i][2]); fflush(stderr); }
      if(intp[i][2]>0.998) intp[i][2]=0.998;
      if(intp[i][1]>0.998) intp[i][1]=0.998;
      if(intp[i][0]>0.998) intp[i][0]=0.998;
      if(intp[i][2]<0.002) intp[i][2]=0.002;
      if(intp[i][1]<0.002) intp[i][1]=0.002;
      if(intp[i][0]<0.002) intp[i][0]=0.002;
    }
    fclose(tab);
    fprintf(stderr,"done!\n");

    if(!debug) err = system("rm -f psipred.dat fasta tmp.mtx");
  }

  pred = fopen(n7, "w");
 
  if(debug) { fprintf(stdout, "Summing probabilities...\n"); fflush(stdout);}

  for(i=0;i<sql;i++)
  {
    pp[i][0] = pp[i][1] = pp[i][2] = pp[i][3] = 0.;
    for(j=0;j<NSHIFT;j++)
    {
       if(Aprob[i][j]<900)
       {
         pp[i][0] += log(Aprob[i][j]);
         pp[i][1] += log(Bprob[i][j]);
         pp[i][2] += log(Cprob[i][j]);
         pp[i][3] += log(Pprob[i][j]);
         if(debug) {fprintf(stderr, "PP SHIFTs A_%i CS_%i H_%8.3f E_%8.3f C_%8.3f P_%8.3f\n", i, j, pp[i][0], pp[i][1], pp[i][2], pp[i][3]); fflush(stderr); }
       }
    }
    /* these are the intrinsic probability per aminoacids */
    if(pp[i][0]!=0.||pp[i][1]!=0.||pp[i][2]!=0.) {
      if(!nopsipred&&pcs[i]>0.&&i>0&&i<sql-1) {
        pp[i][0] += pcs[i]*log(intp[i][0]);
        pp[i][1] += pcs[i]*log(intp[i][1]);
        pp[i][2] += pcs[i]*log(intp[i][2]);
        pp[i][3] += 2.05*pcs[i]*log(intp[i][2]);
        if(debug) {
          fprintf(stderr, "PP PSIPRs A_%i CS_%i H_%8.3f E_%8.3f C_%8.3f P_%8.3f\n", i, j, pcs[i]*log(intp[i][0]), pcs[i]*log(intp[i][1]), 
                                                                                          pcs[i]*log(intp[i][2]), 2.05*pcs[i]*log(intp[i][2])); 
          fflush(stderr); 
        }
      }
      if(debug) {
        fprintf(stderr, "PP TOTALs A_%i CS_%i H_%8.3f E_%8.3f C_%8.3f P_%8.3f\n", i, j, pp[i][0], pp[i][1], pp[i][2], pp[i][3]); 
        fflush(stderr);
      } 
      norm = exp(pp[i][0])+exp(pp[i][1])+exp(pp[i][2]);
      if(ppii) { 
        norm = exp(pp[i][0])+exp(pp[i][1])+exp(pp[i][2])+exp(pp[i][3]);
        pp[i][3] = exp(pp[i][3])/norm;
      }
      pp[i][0] = exp(pp[i][0])/norm;
      pp[i][1] = exp(pp[i][1])/norm;
      pp[i][2] = exp(pp[i][2])/norm;
    }
  }

  fprintf(stderr,"Assigning secondary structure..."); fflush(stderr);

  for(i=0;i<sql;i++)
  {
    if(pp[i][0]!=0||pp[i][1]!=0||pp[i][2]!=0) {
      maxi = pp[i][0]; MAX=0;
      for(j=1;j<3;j++) 
        if( maxi < pp[i][j] ) {maxi = pp[i][j]; MAX=j;}
      if(ppii) {
        maxi = pp[i][0]; MAX=0;
        for(j=1;j<4;j++) if( maxi < pp[i][j] ) {maxi = pp[i][j]; MAX=j;}
      }

      switch(MAX)
      {
        case 0: ss[i]='H'; break;
        case 1: ss[i]='E'; break;
        case 2: ss[i]='C'; break;
        case 3: ss[i]='P'; break;
      }
    }
    else ss[i]=' ';
  }

  fprintf(stderr,"done!\n"); fflush(stderr);
  fprintf(stderr,"SQ:%s \n",seq); fflush(stderr);
  fprintf(stderr,"SS:%s ",ss); fflush(stderr);
  j=0; prec=0;
  for(i=0;i<sql;i++)
  {
    if(pp[i][0]!=0.||pp[i][1]!=0.||pp[i][2]!=0.) {
      if(css[i]=='T') css[i]='C';
      else if(css[i]=='B') css[i]='C';
      else if(css[i]=='G') css[i]='H';
      else if(css[i]=='U') css[i]='C';
      if(css[i]!=ss[i]) prec++;
      if(ss[i]=='P'&&css[i]=='C') prec--;
      j++;
    }
  }
  fprintf(stderr,"%4.1f\n", prec/j*100.);

  /* correction algorithms */
  for(i=0;i<sql;i++)
  {
    if(pp[i][0]!=0.||pp[i][1]!=0.||pp[i][2]!=0.) 
    {
      spp[i][0] = pp[i][0];
      spp[i][1] = pp[i][1];
      spp[i][2] = pp[i][2];
      spp[i][3] = pp[i][3];
      dcount=1.0;

      if(i>1&&(pp[i-2][0]!=0.||pp[i-2][1]!=0.||pp[i-2][2]!=0.)) {
        spp[i][0] += 0.30*pp[i-2][0];
        spp[i][1] += 0.30*pp[i-2][1];
        spp[i][2] += 0.30*pp[i-2][2];
        spp[i][3] += 0.30*pp[i-2][3];
        dcount+=0.30;
      }

      if(i>0&&(pp[i-1][0]!=0.||pp[i-1][1]!=0.||pp[i-1][2]!=0.)) {
        spp[i][0] += 0.70*pp[i-1][0];
        spp[i][1] += 0.70*pp[i-1][1];
        spp[i][2] += 0.70*pp[i-1][2];
        spp[i][3] += 0.70*pp[i-1][3];
        dcount+=0.70;
      }
      if(i<sql-1&&(pp[i+1][0]!=0.||pp[i+1][1]!=0.||pp[i+1][2]!=0.)) {
        spp[i][0] += 0.70*pp[i+1][0];
        spp[i][1] += 0.70*pp[i+1][1];
        spp[i][2] += 0.70*pp[i+1][2];
        spp[i][3] += 0.70*pp[i+1][3];
        dcount+=0.70;
      }

      if(i<sql-2&&(pp[i+2][0]!=0.||pp[i+2][1]!=0.||pp[i+2][2]!=0.)) {
        spp[i][0] += 0.30*pp[i+2][0];
        spp[i][1] += 0.30*pp[i+2][1];
        spp[i][2] += 0.30*pp[i+2][2];
        spp[i][3] += 0.30*pp[i+2][3];
        dcount += 0.30;
      }

      spp[i][0] /= dcount;
      spp[i][1] /= dcount;
      spp[i][2] /= dcount;
      spp[i][3] /= dcount;

      norm = spp[i][0] + spp[i][1] + spp[i][2];
      if(ppii) {
        norm = spp[i][0] + spp[i][1] + spp[i][2] + spp[i][3];
        spp[i][3] /= norm;
      }
      spp[i][0] /= norm;
      spp[i][1] /= norm;
      spp[i][2] /= norm;
    }
    if(pp[i][0]!=0.||pp[i][1]!=0.||pp[i][2]!=0.) {
      maxi = spp[i][0]; MAX=0;
      for(j=1;j<3;j++) 
        if( maxi < spp[i][j] ) {maxi = spp[i][j]; MAX=j;}
      if(ppii) {
        maxi = spp[i][0]; MAX=0;
        for(j=1;j<4;j++) if( maxi < spp[i][j] ) {maxi = spp[i][j]; MAX=j;}
      }

      switch(MAX)
      {
        case 0: if(thread==0.||thread<=maxi) ss[i]='H'; else if(thread>maxi) ss[i]='C'; break;
        case 1: if(thread==0.||thread<=maxi) ss[i]='E'; else if(thread>maxi) ss[i]='C'; break;
        case 2: ss[i]='C'; break;
        case 3: if(thread==0.||thread<=maxi) ss[i]='P'; else if(thread>maxi) ss[i]='C'; break;
      }
    }
  }
  fprintf(stderr,"SS:%s\n",ss); fflush(stderr);

/* Calculating total populations */
  mhelix = mbeta = mcoil = mppii = 0.;
  mtot = 0;
  for(i=0;i<sql;i++)
  {
    if(pp[i][0]!=0||pp[i][1]!=0||pp[i][2]!=0) {
      mhelix += spp[i][0];
      mbeta += spp[i][1];
      mcoil += spp[i][2];
      mtot++;
      if(ppii) mppii += spp[i][3];
    }
  }
  mhelix = mhelix/mtot*100.;
  mbeta = mbeta/mtot*100.;
  mcoil = mcoil/mtot*100.;
  mppii = mppii/mtot*100.;

/* Writing the output file */
  fprintf(pred,"#d2D - v. 1.0.9\n#(c) Carlo Camilloni\n#PLEASE CITE:\n");
  fprintf(pred,"#Camilloni C., De Simone A., ..., and Vendruscolo M.\n");
  fprintf(pred,"#bla title\n");
  fprintf(pred,"#bla journal\n\n");
  fprintf(pred,"#Executed as: ");
  for (i=0;i<argc;i++) fprintf(pred, "%s ", argv[i]);
  fprintf(pred, "\n\n");
  fprintf(pred,"#SQ:%s\n",seq);
  fprintf(pred,"#SS:%s\n\n",ss);
  fprintf(pred,"#Total Populations:\n");
  fprintf(pred,"#Helix(H): %4.1f%%\n", mhelix);
  fprintf(pred,"#Extended-Beta(E): %4.1f%%\n", mbeta);
  if(ppii) fprintf(pred,"#Polyproline II (PPII)(P): %4.1f%%\n", mppii);
  fprintf(pred,"#Coil(C): %4.1f%%\n\n", mcoil);
  fprintf(pred,"#Populations per residue:\n");
  if(ppii) fprintf(pred,"#num \t res \t      Helix       Beta       Coil        PPII  SS \n");
  else     fprintf(pred,"#num \t res \t      Helix       Beta       Coil  SS\n");

  for(i=0;i<sql;i++)
  {
    if(pp[i][0]!=0||pp[i][1]!=0||pp[i][2]!=0) {
      if(!ppii) 
      fprintf(pred, "%i \t %c \t %10.3f %10.3f %10.3f \t %10.3f %10.3f %10.3f %c\n", i+firstres, seq[i], pp[i][0], pp[i][1], pp[i][2], spp[i][0], spp[i][1], spp[i][2], ss[i]);
      else
      fprintf(pred, "%i \t %c \t %10.3f %10.3f %10.3f %10.3f \t %10.3f %10.3f %10.3f  %10.3f %c\n", i+firstres, seq[i], pp[i][0], pp[i][1], pp[i][2], pp[i][3], spp[i][0], spp[i][1], spp[i][2], spp[i][3], ss[i]);
    } else {
      fprintf(pred, "#%i \t %c \t \n", i+firstres, seq[i]);
    }
  }
  fprintf(pred,"#DONE!\n");
  fprintf(pred,"#d2D - v. 1.0.9\n#(c) Carlo Camilloni\n#PLEASE CITE:\n");
  fprintf(pred,"#Camilloni C., De Simone A., ..., and Vendruscolo M.\n");
  fprintf(pred,"#bla title\n");
  fprintf(pred,"#bla journal\n\n");
  fclose(pred);

  j=0; prec=0;
  for(i=0;i<sql;i++)
  {
    if(pp[i][0]!=0||pp[i][1]!=0||pp[i][2]!=0) {
      if(css[i]!=ss[i]) prec++;
      if(ss[i]=='P'&&css[i]=='C') prec--;
      j++;
    }
  }
  fprintf(stderr,"SS:%s\n",css); fflush(stderr);
  fprintf(stdout,"Err %4.1f\n", prec/j*100.);

  return 0;
}

void help()
{
      fprintf(stderr,"SS predictor v. 1.0.9\n");
      fprintf(stderr,"\t-cs        \t([ALL], CA, CB, CO, HN, HA, NH or HNNH)\n");
      fprintf(stderr,"\t-pH        \t([neutral], acid)\n");
      fprintf(stderr,"\t-file      \t(input file name)\n");
      fprintf(stderr,"\t-out       \t(output file name)\n");
      fprintf(stderr,"\t-fres      \t(rescale residue numbers from fres)\n");
      fprintf(stderr,"\t-nopsipred \t(turn off the sequence dependent prediction)\n");
      fprintf(stderr,"\t-ppii      \t(turn on the Polyproline predictor)\n");
      fprintf(stderr,"\t-debug     \t(more verbose)\n");
      fprintf(stderr,"\t-help      \t(here we are!)\n");
}

int main(int argc, char *argv[])
{
  int ashift=NSHIFT;
  int status=0;
  int ph=0;
  int firstres=1;
  int i, j=-1, o=-1;
  char *dbpath;

  dbpath = getenv ("CAMDBPATH");
  if (dbpath==NULL) {fprintf (stderr, "The CAMDBPATH is not set!\n"); return 1;}
  if(argc<3) {help(); return 0;}

  for(i=0;i<argc;i++)
  {
    if(!strcmp(argv[i],"-cs")) {
      if(!strcmp(argv[i+1],"ALL")) ashift = NSHIFT;
      else {
        if(!strcmp(argv[i+1],"CA")) ashift = CA;
        else if(!strcmp(argv[i+1],"CB")) ashift = CB;
        else if(!strcmp(argv[i+1],"CO")) ashift = CO;
        else if(!strcmp(argv[i+1],"HN")) ashift = HN;
        else if(!strcmp(argv[i+1],"HA")) ashift = HA;
        else if(!strcmp(argv[i+1],"NH")) ashift = NH;
        else if(!strcmp(argv[i+1],"HNNH")) ashift = HNNH;
      }
    }
    if(!strcmp(argv[i],"-pH")) {
      if(!strcmp(argv[i+1],"acid")) ph = 1;
      else if(!strcmp(argv[i+1],"neutral")) ph = 0;
    }
    if(!strcmp(argv[i],"-file")) {
      if(argv[i+1]!=NULL) j=i+1;
    }
    if(!strcmp(argv[i],"-out")) {
      if(argv[i+1]!=NULL) o=i+1;
    }
    if(!strcmp(argv[i],"-ss_native")) {
      if(argv[i+1]!=NULL) thread=atof(argv[i+1]);;
    }
    if(!strcmp(argv[i],"-debug")) debug=1;
    if(!strcmp(argv[i],"-nopsipred")) nopsipred=1;
    if(!strcmp(argv[i],"-ppii")) ppii=1;
    if(!strcmp(argv[i],"-fres")) firstres = atoi(argv[i+1]);
    if(!strcmp(argv[i],"-shifty")) shifty=1;
    if(!strcmp(argv[i],"-help")) {help(); return 0;}
  }

  if(debug) {fprintf(stdout, "In main: CAMDBPATH is %s\n", dbpath); fflush(stdout);}
  if(j==-1) {help(); return 0;}

    
  if(argv[j]!=NULL&&o!=-1) status = do_predict(ashift, argv[j], ph, dbpath, argv[o], firstres, argv, argc);
  else if(argv[j]!=NULL&&o==-1) status = do_predict(ashift, argv[j], ph, dbpath, NULL, firstres, argv, argc);
  else {fprintf(stderr, "Wrong file name: %s, expected one\n", argv[j]); return 1;}

  return status;
}  
