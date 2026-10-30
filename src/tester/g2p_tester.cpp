/* g2p_tester.cpp
 *
 * Copyright 2026 Alan Crispin <crispinalan@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

/**
 * @file g2p_tester.cpp
 * @brief Grapheme-to-Phoneme (G2P) model tester and demonstration program
 *
 * This program tests the generated G2P rules by:
 * 1. Loading the prediction functions from g2p_rules.h
 * 2. Converting test words to phonemes
 * 3. Comparing against expected outputs
 */
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include "g2p_rules.h"

using namespace std;

/**
 * @brief Converts a word to its phonemic representation using the trained model
 * @param word Input word to transcribe
 * @return Vector of phonemes (one per grapheme)
 */
vector<string> transcribe(const string& word) {
    vector<string> phonemes;
    
    // Explicitly convert the entire word to lowercase first!
    string clean_word = "";
    for (char ch : word) {
        if (isalpha(ch)) clean_word += tolower(ch);
    }
    
    string padded = "___" + clean_word + "___";
    for (size_t i = 0; i < clean_word.length(); i++) {
        size_t idx = i + 3;
        
        // Double-safeguard all lookahead/lookback context characters
        char p   = tolower(padded[idx - 1]);
        char c   = tolower(padded[idx]);
        char n   = tolower(padded[idx + 1]);
        char nn  = tolower(padded[idx + 2]);
        char nnn = tolower(padded[idx + 3]);
        
        const char* ph = nullptr;
        switch (c) {
            case 'a': ph = predictA(p, c, n, nn, nnn); break;
            case 'b': ph = predictB(p, c, n, nn, nnn); break;
            case 'c': ph = predictC(p, c, n, nn, nnn); break;
            case 'd': ph = predictD(p, c, n, nn, nnn); break;
            case 'e': ph = predictE(p, c, n, nn, nnn); break;
            case 'f': ph = predictF(p, c, n, nn, nnn); break;
            case 'g': ph = predictG(p, c, n, nn, nnn); break;
            case 'h': ph = predictH(p, c, n, nn, nnn); break;
            case 'i': ph = predictI(p, c, n, nn, nnn); break;
            case 'j': ph = predictJ(p, c, n, nn, nnn); break;
            case 'k': ph = predictK(p, c, n, nn, nnn); break;
            case 'l': ph = predictL(p, c, n, nn, nnn); break;
            case 'm': ph = predictM(p, c, n, nn, nnn); break;
            case 'n': ph = predictN(p, c, n, nn, nnn); break;
            case 'o': ph = predictO(p, c, n, nn, nnn); break;
            case 'p': ph = predictP(p, c, n, nn, nnn); break;
            case 'q': ph = predictQ(p, c, n, nn, nnn); break;
            case 'r': ph = predictR(p, c, n, nn, nnn); break;
            case 's': ph = predictS(p, c, n, nn, nnn); break;
            case 't': ph = predictT(p, c, n, nn, nnn); break;
            case 'u': ph = predictU(p, c, n, nn, nnn); break;
            case 'v': ph = predictV(p, c, n, nn, nnn); break;
            case 'w': ph = predictW(p, c, n, nn, nnn); break;
            case 'x': ph = predictX(p, c, n, nn, nnn); break;
            case 'y': ph = predictY(p, c, n, nn, nnn); break;
            case 'z': ph = predictZ(p, c, n, nn, nnn); break;
        }
        
        if (ph && strcmp(ph, "SILENT") != 0) {
            string token_str(ph);
            size_t pos = 0;
            size_t next_pos;
            while ((next_pos = token_str.find(' ', pos)) != string::npos) {
                if (next_pos > pos) {
                    string sub = token_str.substr(pos, next_pos - pos);
                    if (sub != "SILENT") phonemes.push_back(sub);
                }
                pos = next_pos + 1;
            }
            if (pos < token_str.length()) {
                string sub = token_str.substr(pos);
                if (sub != "SILENT") phonemes.push_back(sub);
            }
        }
    }
    return phonemes;
}


/**
 * @brief Tests a single word against expected phonemes and reports results
 * @param word Input word to test
 * @param expected Expected phonemic transcription
 */
void run_test(const string& word, const string& expected) {
    vector<string> pred = transcribe(word);
    string pred_str = "";
    for (auto& s : pred) {
        pred_str += s + " ";
    }
    
    // Clean up trailing spaces from both strings for a fair comparison
    string clean_pred = pred_str;
    while(!clean_pred.empty() && isspace(clean_pred.back())) clean_pred.pop_back();
    while(!clean_pred.empty() && isspace(clean_pred.front())) clean_pred.erase(clean_pred.begin());
    
    string clean_exp = expected;
    while(!clean_exp.empty() && isspace(clean_exp.back())) clean_exp.pop_back();
    while(!clean_exp.empty() && isspace(clean_exp.front())) clean_exp.erase(clean_exp.begin());

    cout << "Word: " << word << "\nExpected: " << clean_exp << "\nPredicted: " << clean_pred << "\n";
    if (clean_pred == clean_exp) {
        cout << "Result: PASS \n\n";
    } else {
        cout << "Result: FAIL \n\n";
    }
}

/**
 * @brief Main program entry point
 * @return Exit status (always 0)
 */
int main() {
    cout << "G2P Tester Program\n==================\n";
    run_test("cat", "K AE T ");
    run_test("fat", "F AE T ");
    run_test("mat", "M AE T "); 
    run_test("rat", "R AE T ");   
    run_test("sat", "S AE T ");   
    run_test("cart", "K AA R T ");
    run_test("cake", "K EY K ");
    run_test("city", "S IH T IY"); 
    run_test("dig", "D IH G ");  
    run_test("dog", "D AO G ");   
    run_test("hello", "HH AH L OW ");
    run_test("kid", "K IH D ");
    return 0;
}
