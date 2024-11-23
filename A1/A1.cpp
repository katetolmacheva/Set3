#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <iomanip>
#include <fstream>

struct Circle {
  double x;
  double y;
  double r;

  bool contains(double px, double py) const {
    double dx = px - x;
    double dy = py - y;
    return (dx * dx + dy * dy) <= (r * r) + 1e-12;
  }
};

double compute_exact_area() {
  double pi = acos(-1.0);
  double S = 0.25 * pi + 1.25 * asin(0.8) - 1.0;
  return S;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::vector<Circle> circles(3);
  for (int i = 0; i < 3; ++i) {
    std::cin >> circles[i].x >> circles[i].y >> circles[i].r;
  }

  double minx = circles[0].x - circles[0].r;
  double maxx = circles[0].x + circles[0].r;
  double miny = circles[0].y - circles[0].r;
  double maxy = circles[0].y + circles[0].r;

  for (int i = 1; i < 3; ++i) {
    minx = std::max(minx, circles[i].x - circles[i].r);
    maxx = std::min(maxx, circles[i].x + circles[i].r);
    miny = std::max(miny, circles[i].y - circles[i].r);
    maxy = std::min(maxy, circles[i].y + circles[i].r);
  }

  if (minx > maxx || miny > maxy) {
    std::cout << "0.000000\n";
    return 0;
  }


  double Srec = (maxx - minx) * (maxy - miny);

  std::mt19937 rng(42);
  std::uniform_real_distribution<double> distx(minx, maxx);
  std::uniform_real_distribution<double> disty(miny, maxy);

  std::ofstream outfile("results.csv");
  outfile << "N,S_approx,Relative_Error\n";
  double Sapprox;
  double Sexact = compute_exact_area();
  for (int N = 100; N <= 100000; N += 500) {
    int M = 0;

    for (int i = 0; i < N; ++i) {
      double px = distx(rng);
      double py = disty(rng);

      bool insideall = true;
      for (int j = 0; j < 3 && insideall; ++j) {
        if (!circles[j].contains(px, py)) {
          insideall = false;
        }
      }
      if (insideall) {
        M++;
      }
    }

    Sapprox = ((double)M / (double)N) * Srec;
    double relativeerror = std::abs(Sexact - Sapprox) / std::abs(Sexact);

    outfile << N << "," << Sapprox << "," << relativeerror << "\n";

    if (N % 10000 == 0) {
      std::cerr << "Processed N = " << N << "\n";
    }
  }

  outfile.close();
  std::cout << Sapprox << "\n";


  return 0;
}