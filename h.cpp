#include <iostream>
#include <vector>
#include <iomanip>
#include <set>
#include <ctime>
#include <cmath>
#include <climits>

using namespace std;

typedef unsigned long int ulli;
typedef set<int> SI;
typedef SI::iterator SetItr;
typedef vector<int> P;

void generateSubset (const SI &S, const ulli b, SI &result)
{
  ulli c = 0;
  for (SetItr itr = S.begin(); itr != S.end(); itr++, c++)
    if ((b & (1 << c)) >> c)
      result.insert(*itr);
}

int p (const int k, const int b)
{
  return -2 * k - 1 + b;
}

int binom (const int n, int k)
{
  if (k > n) return 0;
  if (k * 2 > n) k = n - k;
  if (k == 0) return 1;

  int result = n;

  for (int i = 2; i <= k; ++i) {
    result *= (n - i + 1);
    result /= i;
  }

  return result;
}

bool in (int x, SI S)
{
  return S.find(x) != S.end();
}

int d (int n)
{
  return (n - 1) / 2 - n / 3;
}

bool isValidFixation (const vector<P> &pairs, const SI & fixation)
{
  for (unsigned int i = 0; i < pairs.size(); i++)
    if (!in(pairs[i][0], fixation) && !in(pairs[i][1], fixation))
      return false;

  return true;
}

void generateRemovingRange (const int b, const int m, SI &result)
{
  for (int i = 1; i <= b - 2 * m; i++)
    result.insert(i);
}

void setMinus (const SI &A, const SI &B, SI &result)
{
  for (SetItr itr = A.begin(); itr != A.end(); itr++) 
    if (!in(*itr, B)) 
      result.insert(*itr);
}

void updateCount (const SI &I, const int b, const int n, const int k, ulli &count)
{
  SI R;

  // R = R \cup A(I) \cup B(I) \cup C(I)
  for (SetItr itr = I.begin(); itr != I.end(); itr++) {
    int a = *itr;
    R.insert(b - 2*a);
    if ( (b - a) % 2 == 0 ) R.insert((b - a) / 2);
    SetItr itr2 = itr;
    for (itr2++; itr2 != I.end(); itr2++) R.insert(b - a - *itr2);
  }

  int l = R.size() - I.size();

  if (l > k)
    return;

  vector<P> pairs;
  for (SetItr itr = I.begin(); itr != I.end(); itr++) {
    for (int x = 1; x < b - *itr - x; x++) {
      if (!(in(x, R) || in(b - *itr - x, R)))
      {
        P p;
        p.push_back(x);
        p.push_back(b - *itr - x);
        pairs.push_back(p);
      }
    }
  }

  SI xn;
  generateRemovingRange(b, *(I.begin()), xn);
  SI remainingR;
  setMinus(xn, R, remainingR);

  for (ulli b1 = 0; b1 < pow(2, remainingR.size()); b1++) {
    SI subset;
    generateSubset(remainingR, b1, subset);
    int l_ = l + subset.size();
      
    if (l_ <= k && isValidFixation(pairs, subset))
      count += binom(d(n) - xn.size(), k - l_);
  }
}

int main(int argc, char **argv)
{
  if (argc < 3) {
    cout << "Error: must supply at least 2 arguments: n and k." << endl;
    return 1;
  }

  cout << "Hello world! (github test)\n";
  cout << "Testing out branch\n";
  cout << "Testing out branch\n";
  int n = atoi(argv[1]);
  int k = atoi(argv[2]);
  int b = n % 3;

  if (n <= 24 * k + 12 - 8 * b) {
    cout << "Error: n must strictly greater than " << 24 * k + 12 - 8 * b << "." << endl;
    return 1;
  }

  clock_t start_t, end_t;
  
  /* Start */
  start_t = clock();
  ulli count = 0;
  SI insertingRange;

  for (int i = 0 - (n % 3 == 0); i >= p(k, b); i--)
    insertingRange.insert(i);

  for (ulli b1 = 0; b1 < pow(2, insertingRange.size()); b1++)
  {
    SI I;
    generateSubset(insertingRange, b1, I);
    updateCount(I, b, n, k, count);
  }

  end_t = clock();
  /* End */

  cout << count << endl;
  cout << (float) ((float) end_t - (float) start_t) << "ms to run." << endl;
  return 0;
}
