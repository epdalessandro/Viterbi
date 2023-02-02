#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

//Assumes viterbi table is non-empty
template <typename T>
void printTable(vector<vector<T>> table) {
    for(size_t row = 0; row < table.size(); row++) {
        cout << "[ ";
        for(size_t col = 0; col < table[0].size(); col++) {
            cout << table[row][col] << " ";
        }
        cout << "]\n";
    }
}
template <typename T>
void printTable(vector<T> table) {
    cout << "[ ";
    for(size_t row = 0; row < table.size(); row++) {
        cout << table[row] << " ";
    }
    cout << "]\n";
}

vector<size_t> viterbi(vector<vector<double>> &A, vector<vector<double>> &B, vector<double> &pi, vector<int> &observations) {
    size_t K = observations.size();
    size_t M = A.size();
    vector<vector<double>> viterbiTable(K, vector<double>());
    vector<vector<size_t>> backtrackingTable(K-1, vector<size_t>());
    double observationProb;
    size_t maxRowProbIndex;
    double maxRowProb;
    double currentRowProb;
    for(size_t v = 0; v < M; v++) {
        viterbiTable[0].push_back(pi[v] * B[v][observations[0]]);
    }
    for(size_t k = 1; k < K; k++) {
        for(size_t v = 0; v < M; v++) {
            observationProb = B[v][observations[k]];
            maxRowProb = 0;
            maxRowProbIndex = 0;
            for(size_t u = 0; u < M; u++) {
                currentRowProb = viterbiTable[k-1][u] * A[u][v];
                if(currentRowProb > maxRowProb) {
                    maxRowProb = currentRowProb;
                    maxRowProbIndex = u;
                }
            }
            // cout << "maxRowProb: " << maxRowProb << "\n";
            // cout << "observationProb: " << observationProb << "\n";
            viterbiTable[k].push_back(maxRowProb * observationProb);
            backtrackingTable[k-1].push_back(maxRowProbIndex);
        }
    }
    cout << "Viterbi Table:\n";
    printTable(viterbiTable);

    //Backtracking to obtain most likely hidden state sequence
    double lastHiddenState = viterbiTable[K-1][0];
    vector<size_t> hiddenStates(K, 0);
    hiddenStates[K-1] = 0;
    for(size_t v = 0; v < M; v++) {
        if(viterbiTable[K-1][v] > lastHiddenState) {
            lastHiddenState = viterbiTable[K-1][v];
            hiddenStates[K-1] = v;
        }
    }
    for(size_t k = K-2; k >= 0 && k < K-1; k--) {
        hiddenStates[k] = backtrackingTable[k][hiddenStates[k+1]];
    }

    return hiddenStates;
}

int main() {
    cout << std::setprecision(3);
    
    vector<vector<double>> A{{0.3, 0.7}, {0.2, 0.8}};
    vector<vector<double>> B{{0.3, 0.7}, {0.5, 0.5}};
    vector<double> pi{0.6, 0.4};
    vector<int> observations{0, 0, 1};

    vector<size_t> hiddenStates = viterbi(A, B, pi, observations);

    cout << "Hidden State Sequence:\n";
    printTable(hiddenStates);
}