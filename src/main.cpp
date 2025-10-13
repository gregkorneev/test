#include <bits/stdc++.h>
using namespace std;

enum class Order {
    IJK, IKJ, KIJ, KJI, JIK, JKI
};

static const char* order_name(Order o){
    switch(o){
        case Order::IJK: return "i,j,k";
        case Order::IKJ: return "i,k,j";
        case Order::KIJ: return "k,i,j";
        case Order::KJI: return "k,j,i";
        case Order::JIK: return "j,i,k";
        case Order::JKI: return "j,k,i";
        default: return "?";
    }
}

inline size_t idx(size_t i, size_t j, size_t n){ return i*n + j; }

// Простое заполнение: чтобы детерминировать значения и не «ломать» кэш случайностями
void fill_matrices(vector<double>& A, vector<double>& B, size_t n){
    for(size_t i=0;i<n;i++){
        for(size_t j=0;j<n;j++){
            A[idx(i,j,n)] = (i + j % 7) * 0.001;    // плавные числа
            B[idx(i,j,n)] = (i * 2 + j % 5) * 0.002;
        }
    }
}

void zero_matrix(vector<double>& C){ std::fill(C.begin(), C.end(), 0.0); }

// Реализации шести порядков
void mul(Order ord, const vector<double>& A, const vector<double>& B, vector<double>& C, size_t n){
    zero_matrix(C);
    switch(ord){
        case Order::IJK:
            for(size_t i=0;i<n;i++)
                for(size_t j=0;j<n;j++){
                    double s = 0.0;
                    for(size_t k=0;k<n;k++)
                        s += A[idx(i,k,n)] * B[idx(k,j,n)];
                    C[idx(i,j,n)] = s;
                }
            break;
        case Order::IKJ:
            for(size_t i=0;i<n;i++)
                for(size_t k=0;k<n;k++){
                    double aik = A[idx(i,k,n)];
                    for(size_t j=0;j<n;j++)
                        C[idx(i,j,n)] += aik * B[idx(k,j,n)];
                }
            break;
        case Order::KIJ:
            for(size_t k=0;k<n;k++)
                for(size_t i=0;i<n;i++){
                    double aik = A[idx(i,k,n)];
                    for(size_t j=0;j<n;j++)
                        C[idx(i,j,n)] += aik * B[idx(k,j,n)];
                }
            break;
        case Order::KJI:
            for(size_t k=0;k<n;k++)
                for(size_t j=0;j<n;j++){
                    double bkj = B[idx(k,j,n)];
                    for(size_t i=0;i<n;i++)
                        C[idx(i,j,n)] += A[idx(i,k,n)] * bkj;
                }
            break;
        case Order::JIK:
            for(size_t j=0;j<n;j++)
                for(size_t i=0;i<n;i++){
                    double s = 0.0;
                    for(size_t k=0;k<n;k++)
                        s += A[idx(i,k,n)] * B[idx(k,j,n)];
                    C[idx(i,j,n)] = s;
                }
            break;
        case Order::JKI:
            for(size_t j=0;j<n;j++)
                for(size_t k=0;k<n;k++){
                    double bkj = B[idx(k,j,n)];
                    for(size_t i=0;i<n;i++)
                        C[idx(i,j,n)] += A[idx(i,k,n)] * bkj;
                }
            break;
    }
}

// Возвращает медиану времени (мс) из repeats прогонов
double time_ms(function<void()> fn, int repeats){
    vector<double> t;
    t.reserve(repeats);
    for(int r=0;r<repeats;r++){
        auto t0 = chrono::high_resolution_clock::now();
        fn();
        auto t1 = chrono::high_resolution_clock::now();
        double ms = chrono::duration<double, milli>(t1 - t0).count();
        t.push_back(ms);
    }
    nth_element(t.begin(), t.begin()+t.size()/2, t.end());
    return t[t.size()/2];
}

// Парсинг опциональных аргументов:
//   --sizes 128,256,384,512
//   --repeats 3
// По умолчанию sizes = 128,256,384,512,640; repeats = 3
int main(int argc, char** argv){
    ios::sync_with_stdio(false);

    vector<size_t> sizes = {128, 256, 384, 512, 640};
    int repeats = 3;

    for(int i=1;i<argc;i++){
        string a = argv[i];
        if(a=="--sizes" && i+1<argc){
            sizes.clear();
            string s = argv[++i];
            string cur;
            for(char c: s){
                if(c==','){ if(!cur.empty()){ sizes.push_back(stoul(cur)); cur.clear(); } }
                else cur.push_back(c);
            }
            if(!cur.empty()) sizes.push_back(stoul(cur));
        } else if(a=="--repeats" && i+1<argc){
            repeats = stoi(argv[++i]);
        }
    }

    vector<Order> orders = {
        Order::IJK, Order::IKJ, Order::KIJ, Order::KJI, Order::JIK, Order::JKI
    };

    cout << "order,n,time_ms,GFLOPS\n";

    for(size_t n: sizes){
        vector<double> A(n*n), B(n*n), C(n*n);
        fill_matrices(A, B, n);

        // «разогрев» кэшей и JIT отсутствует, но полезно прогреть память
        mul(Order::IKJ, A, B, C, n);

        for(Order ord: orders){
            auto runner = [&](){ mul(ord, A, B, C, n); };
            double ms = time_ms(runner, repeats);

            // теоретическое число операций умножения матриц ~ 2*n^3 FLOP
            double gflops = (2.0 * n * n * n) / (ms / 1000.0) / 1e9;

            cout << order_name(ord) << "," << n << "," << fixed << setprecision(3)
                 << ms << "," << setprecision(2) << gflops << "\n";
        }
    }
    return 0;
}