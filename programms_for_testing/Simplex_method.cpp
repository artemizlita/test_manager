#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <math.h>

using namespace std;

int main(int argc, char *argv[])
{
    int n, m, task_min;
    vector<double> T_b_s, T_c_s, signs;
    vector<vector <double> > T_A_s;

    vector<vector <double> > Coefficients;
    vector<double> Basic_variables_values, Indexes, Taus, M, c;
    vector<int> Basic_variables;
    double F, F_M;
    bool optimal, no_decision;

    ifstream fin;
        fin.open("in.txt");
        fin >> n;
        cout << n << "\n";
        fin >> m;
        cout << m << "\n";

        T_c_s.resize(n);
        for (int j = 0; j < n; ++j) {
            fin >> T_c_s[j];
            cout << T_c_s[j] << " ";
        }

        fin >> task_min;
        cout << task_min << "\n";

        T_b_s.resize(m);
        T_A_s.resize(m);
        signs.resize(m);
        for (int i = 0; i < m; ++i) {
            T_A_s[i].resize(n);
            for (int j = 0; j < n; ++j) {
                fin >> T_A_s[i][j];
                cout << T_A_s[i][j] << " ";
            }
            fin >> signs[i];
            //signs[i] = T_A_s[i][T_A_s[i].size() - 2];
            cout << "z" << signs[i] << "z ";
            fin >> T_b_s[i];
            //T_b_s[i] = T_A_s[i][T_A_s[i].size() - 1];
            cout << T_b_s[i] << " ";
            cout << "\n";
            //T_A_s[i].erase(T_A_s[i].begin() + T_A_s[i].size() - 1);
            //T_A_s[i].erase(T_A_s[i].begin() + T_A_s[i].size() - 1);
        }
        fin.close();

    Coefficients.resize(m);

    int String_size = n;

        for (int i = 0; i < m; ++i) {
            if (T_b_s[i] >= 0) {
                if (signs[i] == 0) {
                    String_size += 2;
                } else {
                    String_size += 1;
                }
            } else {
                if (signs[i] == 1) {
                    String_size += 2;
                } else {
                    String_size += 1;
                }
            }
        }

        for (int i = 0; i < m; ++i) {
            Coefficients[i].resize(String_size);
            for (int j = 0; j < n; ++j) {
                Coefficients[i][j] = T_A_s[i][j];
            }
        }

        int next_coef = n, Add_coef = 0;
        F = 0;
        Basic_variables.resize(m);
        c.clear();

        for (int i = 0; i < n; ++i) {
            if (task_min == 0) {
                c.push_back(T_c_s[i]);
            } else {
                c.push_back(-T_c_s[i]);
            }
        }

        vector <short int> Add_Variable;
        for (int i = 0; i < m; ++i) {
            Add_Variable.push_back(0);
        }

        for (int i = 0; i < m; ++i) {
            if (signs[i] == 0) {
                for (int j = 0; j < m; ++j) {
                    Coefficients[j][next_coef] = (i == j) ? -1 : 0;
                }
                Add_Variable[i] = -1;
                ++Add_coef;
                ++next_coef;
            } else if (signs[i] == 1) {
                //F += Basic_variables_values[i];
                for (int j = 0; j < m; ++j) {
                    Coefficients[j][next_coef] = (i == j) ? 1 : 0;
                    if (i == j) {
                        Basic_variables[i] = next_coef + 1;
                    }
                }
                Add_Variable[i] = 1;
                ++Add_coef;
                ++next_coef;
            }
        }

        Basic_variables_values.resize(m);
        for (int i = 0; i < m; ++i) {
            if (T_b_s[i] < 0) {
                Basic_variables_values[i] = -T_b_s[i];
                Add_Variable[i] = -Add_Variable[i];
                for (int j = 0; j < String_size; ++j) {
                    Coefficients[i][j] = -Coefficients[i][j];
                }
            } else {
                Basic_variables_values[i] = T_b_s[i];
            }
        }

        F_M = 0;

        M.resize(String_size);
        for (int i = 0; i < String_size; ++i) {
            M[i] = 0;
        }

        for (int i = 0; i < m; ++i) {
            if (Add_Variable[i] != 1) {
                for (int j = 0; j < m; ++j) {
                    Coefficients[j][next_coef] = (i == j) ? 1 : 0;
                    if (i == j) {
                        Basic_variables[i] = next_coef + 1;
                    }
                }
                ++next_coef;
                F_M -= Basic_variables_values[i];
                for (int j = 0; j < n + Add_coef; ++j) {
                    M[j] += Coefficients[i][j];
                }
            }
        }

        Indexes.resize(String_size);

        for (int i = 0; i < c.size(); ++i) {
            Indexes[i] = c[i];
        }

        for (int i = c.size(); i < String_size; ++i) {
            Indexes[i] = 0;
        }

        optimal = 0;

        for (int i = 0; i < n + m; ++i) {
            if (Indexes[i] < 0) {
                optimal = 0;
            }
        }

        double max = M[0] * 32000 + Indexes[0];
        int k = 1;

        for (int i = 1; i < String_size; ++i) {
            if (M[i] * 32000 + Indexes[i] > M[k - 1] * 32000 + Indexes[k - 1]) {
                max = M[i] * 32000 + Indexes[i];
                k = i + 1;
            }
        }

        if (max <= 0) {
            optimal = 1;
        }

        no_decision = 1;

        Taus.resize(m);
        int l;
        bool first = 1;
        double min;

        for (int i = 0; i < m; ++i) {
            if (Coefficients[i][k - 1] > 0) {
                Taus[i] = Basic_variables_values[i] / Coefficients[i][k - 1];
                if (!first) {
                    if (Taus[i] < min) {
                        l = i + 1;
                        min = Taus[i];
                    }
                } else {
                    l = i + 1;
                    min = Taus[i];
                }
                first = 0;
                no_decision = 0;
            }
        }

        while (!optimal && !no_decision) {
            Basic_variables[l - 1] = k;

            Basic_variables_values[l - 1] /= Coefficients[l - 1][k - 1];

            double coef = Coefficients[l - 1][k - 1];
            for (int i = 0; i < String_size; ++i) {
                Coefficients[l - 1][i] /= coef;
            }

            for (int i = 0; i < m; ++i) {
                if (i != l - 1) {
                    coef = Coefficients[i][k - 1];
                    Basic_variables_values[i] -= Basic_variables_values[l - 1] * coef;
                    for (int j = 0; j < String_size; ++j) {
                        Coefficients[i][j] -= Coefficients[l - 1][j] * coef;
                    }
                }
            }

            F = 0;
            F_M = 0;

            for (int i = 0; i < m; ++i) {
                if (Basic_variables[i] > n + Add_coef) {
                    F_M -= Basic_variables_values[i];
                } else if (Basic_variables[i] - 1 < n) {
                    F += Basic_variables_values[i] * c[Basic_variables[i] - 1];
                }
            }

            for (int i = 0; i < String_size; ++i) {
                Indexes[i] = (i < c.size()) ? c[i] : 0;
                M[i] = (i + 1 > n + Add_coef) ? -1 : 0;
            }

            for (int i = 0; i < m; ++i) {
                if (Basic_variables[i] - 1 < c.size()) {
                    for (int j = 0; j < String_size; ++j) {
                        Indexes[j] -= Coefficients[i][j] * c[Basic_variables[i] - 1];
                    }
                } else if (Basic_variables[i] > n + Add_coef) {
                    for (int j = 0; j < String_size; ++j) {
                        M[j] += Coefficients[i][j];
                    }
                }
            }

            optimal = 0;

            max = M[0] * 32000 + Indexes[0];
            k = 1;

            for (int i = 1; i < String_size; ++i) {
                if (M[i] * 32000 + Indexes[i] > M[k - 1] * 32000 + Indexes[k - 1]) {
                    max = M[i] * 32000 + Indexes[i];
                    k = i + 1;
                }
            }

            if (max <= 0) {
                optimal = 1;
            }

            no_decision = 1;

            Taus.resize(m);
            first = 1;

            for (int i = 0; i < m; ++i) {
                if (Coefficients[i][k - 1] > 0) {
                    Taus[i] = Basic_variables_values[i] / Coefficients[i][k - 1];
                    if (!first) {
                        if (Taus[i] < min) {
                            l = i + 1;
                            min = Taus[i];
                        }
                    } else {
                        l = i + 1;
                        min = Taus[i];
                    }
                    no_decision = 0;
                    first = 0;
                }
            }
        }

        //string s;

        ofstream fout;
            fout.open("out.txt");

        if (F_M != 0) {

            //s = "incorrect constraints";
            fout << "incorrect constraints";
            cout << "incorrect constraints";

        } else if (optimal) {

            /*s = "X=(";
            for (int i = 0; i < n; ++i) {
                bool basic = 0;
                for (int j = 0; j < m; ++j) {
                    if (Basic_variables[j] == i + 1) {
                        s += QString().setNum(Basic_variables_values[j], 'f', 2) + "; ";
                        basic = 1;
                    }
                }
                if (!basic) {
                    s += "0; ";
                }
            }

            s.remove(s.length() - 2, 2);
            s+= ") ";

            if (ui->comboBox->currentIndex() == 0) {
                s += "F(x)=" + QString().setNum(F + ui->textEdit_3->toPlainText().toDouble(), 'f', 2);
            } else {
                s += "F(x)=" + QString().setNum(-F + ui->textEdit_3->toPlainText().toDouble(), 'f', 2);
            }*/
            //fout << "X=(";
            //cout << "X=(";
            for (int i = 0; i < n; ++i) {
                bool basic = 0;
                for (int j = 0; j < m; ++j) {
                    if (Basic_variables[j] == i + 1) {
                        fout << round(Basic_variables_values[j] * 100) / 100.0 << " ";
                        cout << round(Basic_variables_values[j] * 100) / 100.0 << " ";
                        basic = 1;
                    }
                }
                if (!basic) {
                    fout << "0 ";
                    cout << "0 ";
                }
            }
            //fout << ") ";
            //cout << ") ";

            /*if (task_min == 0) {
                fout << "F(x)=" <<

            } else {

            }*/

        } else {

            /*s = "no optimal plan";*/
            fout << "no optimal plan";
            cout << "no optimal plan";

        }

        fout.close();

        //L_xn_sm->setText(s);
    return 0;
}

