#include <iostream>
#include <string>
using namespace std;
//纯手搓复现一次完整的 KMP 算法
void buildLPS(string Pattern, int lps[]) {
    lps[0] = 0;

    int m = Pattern.length();
    int prefixLength = 0;
    int current = 1;
    while (current < m) {
        if (Pattern[prefixLength] == Pattern[current]) {
            prefixLength++;
            lps[current] = prefixLength;
            current ++;
        }else {
            if (prefixLength > 0) {
                prefixLength = lps[prefixLength - 1];
            }else {
                lps[current] = 0;
                current ++;
            }
        }
    }
}

int using_KMP(string text, string pattern){
    int m = text.length();
    int n = pattern.length();
    int i = 0;
    int j = 0;
    int lps[n] ;
    buildLPS(pattern, lps);
    while (i<m) {
        //int count = 0;//当前版本count是有问题的
        while (text[i] == pattern[j]) {
            i++;
            j++;
            //count++;
            if (j == n) {
                return i - n;
            }
        }
        // if (count==n) {
        //     return i-count;
        // }
        if (text[i] != pattern[j]) {
            if (j>0) {
                j = lps[j-1];
            }else {
                i++;
            }
        }
    }
    return -1;
}

int main() {
    string Text;
    cout << "Please enter a text: ";
    cin >> Text;
    string Pattern;
    cout << "Please enter the pattern: ";
    cin >> Pattern;
    int index = using_KMP(Text, Pattern);
    cout << "The first time pattern occurs:" << index <<endl;
}