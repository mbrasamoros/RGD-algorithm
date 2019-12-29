/*
 *
 * Copyright (c) 2019 Maria Bras-Amoros, Julio Fernandez-Gonzalez
 *
 * Distributed under the terms of the GNU General Public License
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details.
 * The full text is available at http://www.gnu.org/licenses/
 *
 * Last update: October 29, 2019
 *
 */

#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
using namespace std;

uint_fast64_t Dlength;

unsigned long long int RGD(unsigned char D[], uint_fast64_t m, uint_fast64_t u, uint_fast64_t c, uint_fast64_t gdiffminustwo, uint_fast64_t r) {
  unsigned long long int ng = 0;
  unsigned char newD[Dlength];
  uint_fast64_t s, l, Frob, aux = c - m + u;

  if (gdiffminustwo) {
    memset(newD + c, 0, m);
    memcpy(newD, D, c);
    --gdiffminustwo;
    for (s = c - m; s < aux; s++) {
      if (newD[s]) {
        Frob = s + m;
        l = s / 2;
        while (l >= u && (newD[l] || newD[s - l]))
          l--;
        if (l < u) {
          newD[Frob] = 1;
          ng += RGD(newD, m, u, Frob + 1, gdiffminustwo, r--);
          newD[Frob] = 0;
        } else
          ng += RGD(newD, m, u, Frob + 1, gdiffminustwo, --r);
        newD[s] = 0;
      }
    }
    for (; r > 1; s++) {
      if (newD[s]) {
        ng += RGD(newD, m, u, s + m + 1, gdiffminustwo, --r);
        newD[s] = 0;
      }
    }
  } else {
    for (s = c - m; s < aux; s++) {
      if (D[s]) {
        l = s / 2;
        while (l >= u && (D[l] || D[s - l]))
          l--;
        if (l < u)
          ng += r--;
        else
          ng += --r;
      }
    }
    while (r)
      ng += --r;
  }
  return ng;
}

unsigned long long int pseudo(unsigned char D[], uint_fast64_t m, uint_fast64_t u, uint_fast64_t r, uint_fast64_t gamma) {
  unsigned long long int ng = 0;
  unsigned char newD[Dlength];
  uint_fast64_t s, Frob, c = u + m, gdiffminustwo, tu = 2 * u;

  memset(newD + c, 0, m);
  memcpy(newD, D, c);
  newD[u] = 0;
  gdiffminustwo = gamma - c - 1;

  if (tu <= m) {
    for (s = u + 1; s < tu; s++) {
      Frob = s + m;
      newD[Frob] = 1;
      ng += RGD(newD, m, u, Frob + 1, gdiffminustwo, r--);
      newD[Frob] = 0;
      newD[s] = 0;
    }
    for (s = tu; s < m; s++) {
      ng += RGD(newD, m, u, s + m + 1, gdiffminustwo, --r);
      newD[s] = 0;
    }
    for (s = m + 1; s < c; s++) {
      ng += RGD(newD, m, u, s + m + 1, gdiffminustwo, --r);
      newD[s] = 0;
    }
  } else {
    for (s = u + 1; s < m; s++) {
      Frob = s + m;
      newD[Frob] = 1;
      ng += RGD(newD, m, u, Frob + 1, gdiffminustwo, r--);
      newD[Frob] = 0;
      newD[s] = 0;
    }
    for (s = m + 1; s < tu; s++) {
      Frob = s + m;
      newD[Frob] = 1;
      ng += RGD(newD, m, u, Frob + 1, gdiffminustwo, r--);
      newD[Frob] = 0;
      newD[s] = 0;
    }
    for (s = tu; s < c; s++) {
      ng += RGD(newD, m, u, s + m + 1, gdiffminustwo, --r);
      newD[s] = 0;
    }
  }
  return ng;
}

int main(int numvars, char **vars) {
  unsigned long long int ng;
  uint_fast64_t gamma, arraylength;
  time_t start, stop;

  __cilkrts_set_param("nworkers", "8");

  gamma = (uint_fast64_t)atoi(vars[1]);

  if (gamma < 4) {
    printf("The target genus must be at least 4\n");
    exit(0);
  }

  start = time(NULL);

  arraylength = gamma - 1;
  Dlength = 3 * (gamma - 3);

  unsigned char D[arraylength][arraylength][Dlength];
  unsigned long long int ngmu[arraylength][arraylength];
  uint_fast64_t r[arraylength], gammaminusm[arraylength], aux[arraylength];

  ng = 2 * gamma - 1 + (gamma - 4) * (gamma - 3) / 2; //cases m=2, m=gamma-1, m=gamma, m=gamma+1

  cilk_for(uint_fast64_t m = 3; m < gamma - 1; m++) {

    /* BEGIN PSEUDO-ORDINARIES */
    gammaminusm[m] = gamma - m;

    aux[m] = m - 2;
    if (gammaminusm[m] > m) {
      cilk_for(uint_fast64_t u = 2; u < m; u++) {
        memset(D[m][u], 1, m + u);
        D[m][u][m] = 0;
        ngmu[m][u] += pseudo(D[m][u], m, u, aux[m], gamma);
      }
      memset(D[m][m], 1, m + m);
      D[m][m][m] = 0;
      ngmu[m][m] += pseudo(D[m][m], m, m, m - 1, gamma);
    } else {
      cilk_for(uint_fast64_t u = 2; u < gammaminusm[m]; u++) {
        memset(D[m][u], 1, m + u);
        D[m][u][m] = 0;
        ngmu[m][u] += pseudo(D[m][u], m, u, aux[m], gamma);
      }
      ngmu[m][gammaminusm[m]] += gammaminusm[m] + (m - 1) * aux[m] / 2;
      if (2 * gammaminusm[m] > m)
        ngmu[m][gammaminusm[m]] -= 1;
    }
    /* END PSEUDO-ORDINARIES */

    /* BEGIN QUASI-ORDINARIES */
    memset(D[m][1], 1, m);
    memset(D[m][1] + m, 0, m);
    D[m][1][1] = 0;
    r[m] = aux[m];
    aux[m] = gammaminusm[m] - 2;
    for (uint_fast64_t splusone = 3; splusone < m; splusone++) { //splusone is s shifted by one
      ngmu[m][1] += RGD(D[m][1], m, 1, splusone + m, aux[m], --r[m]);
      D[m][1][splusone - 1] = 0;
    }
    /* END QUASI-ORDINARIES */
  }

  for (uint_fast64_t m = 3; m < gamma - 1; m++)
    for (uint_fast64_t u = 1; u <= m; u++)
      ng += ngmu[m][u];

  stop = time(NULL);

  printf("n%d=%lld\n", (int)gamma, ng);
  printf("time taken %d (%d workers)\n", (int)(stop - start), __cilkrts_get_nworkers());
  return 0;
}
