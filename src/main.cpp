#include <iostream>
#include <chrono>
#include <random>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>

// ---------------- Matrix helpers ----------------
double** createMatrix(int n) {
    double** matrix = new double*[n];
    for (int i = 0; i < n; ++i) matrix[i] = new double[n];
    return matrix;
}

void deleteMatrix(double** matrix, int n) {
    for (int i = 0; i < n; ++i) delete[] matrix[i];
    delete[] matrix;
}

void initializeMatrices(double** A, double** B, double** C, int n) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            B[i][j] = dis(gen);
            C[i][j] = dis(gen);
            A[i][j] = 0.0;
        }
    }
}

// ---------------- Multiplication orders ----------------
void multiply_ijk(double** A, double** B, double** C, int n) {
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) {
            A[i][j] = 0.0;
            for (int k = 0; k < n; ++k)
                A[i][j] += B[i][k] * C[k][j];
        }
}

void multiply_ikj(double** A, double** B, double** C, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) A[i][j] = 0.0;
        for (int k = 0; k < n; ++k)
            for (int j = 0; j < n; ++j)
                A[i][j] += B[i][k] * C[k][j];
    }
}

void multiply_kij(double** A, double** B, double** C, int n) {
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) A[i][j] = 0.0;
    for (int k = 0; k < n; ++k)
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                A[i][j] += B[i][k] * C[k][j];
}

void multiply_kji(double** A, double** B, double** C, int n) {
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) A[i][j] = 0.0;
    for (int k = 0; k < n; ++k)
        for (int j = 0; j < n; ++j)
            for (int i = 0; i < n; ++i)
                A[i][j] += B[i][k] * C[k][j];
}

void multiply_jik(double** A, double** B, double** C, int n) {
    for (int j = 0; j < n; ++j)
        for (int i = 0; i < n; ++i) {
            A[i][j] = 0.0;
            for (int k = 0; k < n; ++k)
                A[i][j] += B[i][k] * C[k][j];
        }
}

void multiply_jki(double** A, double** B, double** C, int n) {
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) A[i][j] = 0.0;
    for (int j = 0; j < n; ++j)
        for (int k = 0; k < n; ++k)
            for (int i = 0; i < n; ++i)
                A[i][j] += B[i][k] * C[k][j];
}

// ---------------- Timing ----------------
double measureTime(void (*multiplyFunc)(double**, double**, double**, int),
                   double** A, double** B, double** C, int n) {
    auto start = std::chrono::high_resolution_clock::now();
    multiplyFunc(A, B, C, n);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    return duration.count(); // seconds
}

// Format helper: convert dot to comma for RU Excel
std::string fmt(double x, int prec = 6) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(prec) << x;
    std::string s = ss.str();
    std::replace(s.begin(), s.end(), '.', ',');
    return s;
}

// ---------------- Benchmark one size ----------------
void benchmarkForSize(int n, int repetitions, std::ofstream& csv) {
    std::cout << "\nMatrix size: " << n << "x" << n << "\n"
              << "==========================================\n";

    double** A = createMatrix(n);
    double** B = createMatrix(n);
    double** C = createMatrix(n);
    initializeMatrices(A, B, C, n);

    auto avg = [&](auto f){ double tot=0; for(int r=0;r<repetitions;++r) tot+=measureTime(f,A,B,C,n); return tot/repetitions; };

    const double avg_ijk = avg(multiply_ijk);
    const double avg_ikj = avg(multiply_ikj);
    const double avg_kij = avg(multiply_kij);
    const double avg_kji = avg(multiply_kji);
    const double avg_jik = avg(multiply_jik);
    const double avg_jki = avg(multiply_jki);

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "(i,j,k): " << avg_ijk << "s\n";
    std::cout << "(i,k,j): " << avg_ikj << "s\n";
    std::cout << "(k,i,j): " << avg_kij << "s\n";
    std::cout << "(k,j,i): " << avg_kji << "s\n";
    std::cout << "(j,i,k): " << avg_jik << "s\n";
    std::cout << "(j,k,i): " << avg_jki << "s\n";

    auto gflops = [&](double t){ return (2.0 * n * n * n) / t / 1e9; };

    // Write CSV with semicolon separator for RU Excel
    csv << "i,j,k;" << n << ";" << fmt(avg_ijk) << ";" << fmt(gflops(avg_ijk)) << "\n";
    csv << "i,k,j;" << n << ";" << fmt(avg_ikj) << ";" << fmt(gflops(avg_ikj)) << "\n";
    csv << "k,i,j;" << n << ";" << fmt(avg_kij) << ";" << fmt(gflops(avg_kij)) << "\n";
    csv << "k,j,i;" << n << ";" << fmt(avg_kji) << ";" << fmt(gflops(avg_kji)) << "\n";
    csv << "j,i,k;" << n << ";" << fmt(avg_jik) << ";" << fmt(gflops(avg_jik)) << "\n";
    csv << "j,k,i;" << n << ";" << fmt(avg_jki) << ";" << fmt(gflops(avg_jki)) << "\n";

    double min_time = avg_ijk; std::string best = "(i,j,k)";
    auto upd = [&](double t,const char* nstr){ if (t<min_time){ min_time=t; best=nstr; } };
    upd(avg_ikj,"(i,k,j)"); upd(avg_kij,"(k,i,j)"); upd(avg_kji,"(k,j,i)"); upd(avg_jik,"(j,i,k)"); upd(avg_jki,"(j,k,i)");
    std::cout << "Best method: " << best << " (" << min_time << "s)\n";

    deleteMatrix(A, n); deleteMatrix(B, n); deleteMatrix(C, n);
}

int main() {
    std::cout << "Matrix Multiplication Performance Comparison\n"
              << "==========================================\n";

    // CSV in working directory
    std::ofstream csv("results.csv");
    // Header with semicolons
    csv << "order;n;avg_time_sec;gflops\n";

    const int sizes[]       = {20, 100, 500, 1000};
    const int repetitions[] = {5,   3,   2,   1   };

    for (int i = 0; i < 4; ++i)
        benchmarkForSize(sizes[i], repetitions[i], csv);

    csv.close();
    std::cout << "\nРезультаты сохранены в файл: results.csv\n";

    std::cout << "\n=== Analysis (short) ===\n"
              << "- Перестановка циклов не меняет математику, но меняет паттерн доступа к памяти.\n"
              << "- Порядки, которые проходят по строкам/столбцам последовательно, лучше используют кэш.\n";
    return 0;
}
