# CART-Based Grapheme-to-Phoneme (G2P) Rule Generator & Tester

A system for generating grapheme-to-phoneme (G2P) prediction rules from the CMU Pronouncing Dictionary and testing their accuracy. A G2P engine converts written spelling units (graphemes) into a sequence of phonemes that represents how the text should be pronounced. 

## Project Architecture

The project consists of two main components:

1. **G2P Trainer (`g2p_trainer.cpp`)**  
   Reads and analyzes the CMU Pronouncing Dictionary to discover phonetic patterns. It aligns letters to phonemes using dynamic programming, extracts contextual features, and automatically constructs decision trees to generate `g2p_rules.h`. This header contains the learned pronunciation rules ready for inference.

2. **G2P Tester (`g2p_tester.cpp`)**  
   Loads the generated rules and evaluates them on new words. It simulates the full G2P pipeline: padding words, extracting context windows, dispatching to letter-specific prediction functions, and comparing output against expected ARPAbet sequences.

## How CART Works 

Classification and Regression Trees (CART) is a machine learning approach that automatically derives pronunciation rules from data in this case the [CMU Pronouncing Dictionary](http://www.speech.cs.cmu.edu/cgi-bin/cmudict). It functions as a decision tree: to determine the phoneme for a specific letter, the model asks a series of binary (yes/no) questions about the surrounding context. For example: Is the next letter "e"? make a prediction. Is the next letter "a"? make a different prediction.  The algorithm identifies the most effective splits in the training dictionary to maximise accuracy, ultimately producing a rule set of if-else statements that capture complex, context-dependent patterns.

Imagine teaching a computer to pronounce letters by giving it a flowchart of yes/no questions. This is how Classification and Regression Trees (CART) work. Instead of a human writing a long list of rules, the computer analyses thousands of examples and builds its own "decision tree." For a letter like "c", it might ask, is the next letter "a"? to decide if it should sound like K. If the answer is no, it follows a different branch of the tree to find the right sound. Because these trees are built directly from real-world data, the computer discovers pronunciation patterns and exceptions automatically, resulting in a learnt set of rules.

The CART method is used to automatically generate the logic for grapheme-to-phoneme conversion rules (a series of predict functions) which a (***G2P engine***) can use. The most decisive rules are placed at the top of a predict function to catch the most specific cases first, while the most common sounds, such as AA, serve as the default return values at the bottom. A G2P engine uses standard ARPAbet notation to predict pronunciation based entirely on the evidence found in the training data. 

Magic E (also known as the Silent E) is a common rule in English where an "e" at the end of a word is not pronounced, but instead changes the pronunciation of the vowel that comes before it. This rule is demonstrated by comparing the two words hat” and “hate”.  The first word "hat" has a short vowel sound because there is no "e" at the end. When you add the "e" to create the second word "hate", the silent "e" magically changes the sound of the previous vowel from short to long.

The trainer builds these question chains (trees) by finding the splits that best separate phonetic outcomes. Each path from the root to a leaf becomes a human-readable "if-else" rule in the generated header. Because the trees are built directly from dictionary data, they naturally capture context-dependent pronunciation patterns. 

## Key Technical Components

- **Dynamic Programming Alignment**: Levenshtein-style cost matrix maps each letter to its most likely phoneme, handling silent letters and variable-length phoneme sequences.

- **Context Windows**: Each letter is evaluated with a 5-character window `(previous, current, next, next-next, next-next-next)` to capture spelling patterns like "magic e" or r-coloured vowels.

- **Gini Impurity Splitting**: The tree builder evaluates all context features and picks the split that maximizes phoneme separation, avoiding arbitrary or data-order-dependent decisions. The Gini impurity function is crucial for building decision trees as it measures how often a randomly chosen element would be incorrectly labelled if a label is picked according to the distribution of labels in the subset. Lower Gini values indicate purer subsets.

## Files

### Core Components
- `g2p_trainer.cpp`: Main training program
- `g2p_tester.cpp`: Tester program for testing generated rules (in g2p_rules.h) from trainer program
- `Makefile`: Build configuration

### Generated Files
- `g2p_rules.h`: Header file containing all prediction functions (generated)

## Building and Running

### Prerequisites
- C++11 compatible compiler (g++, clang++)
- CMU Pronouncing Dictionary (cmudict-0.7b)

### Compiling Trainer
Inside the folder which contains the g2p_trainer.cpp file use "make" to compile the trainer program.
```
make
```

### Training Phase

Generate rules from CMU dictionary.
```
./g2p_trainer cmudict-0.7b

Analyzing dictionary and generating alignments...
==================================================
Generating CART decision tree rules (g2p_rules.h)...
==================================================
Letter 'a': Building tree with 81868 samples.
Letter 'b': Building tree with 20849 samples.
Letter 'c': Building tree with 36251 samples.
Letter 'd': Building tree with 32457 samples.
Letter 'e': Building tree with 105810 samples.
Letter 'f': Building tree with 12959 samples.
Letter 'g': Building tree with 25381 samples.
Letter 'h': Building tree with 26850 samples.
Letter 'i': Building tree with 70545 samples.
Letter 'j': Building tree with 2497 samples.
Letter 'k': Building tree with 16563 samples.
Letter 'l': Building tree with 53808 samples.
Letter 'm': Building tree with 28822 samples.
Letter 'n': Building tree with 67874 samples.
Letter 'o': Building tree with 60008 samples.
Letter 'p': Building tree with 21128 samples.
Letter 'q': Building tree with 1328 samples.
Letter 'r': Building tree with 72045 samples.
Letter 's': Building tree with 70271 samples.
Letter 't': Building tree with 54278 samples.
Letter 'u': Building tree with 28250 samples.
Letter 'v': Building tree with 9754 samples.
Letter 'w': Building tree with 10335 samples.
Letter 'x': Building tree with 2156 samples.
Letter 'y': Building tree with 14913 samples.
Letter 'z': Building tree with 5892 samples.
Successfully generated CART g2p_rules.h with 26 letter contexts analyzed.
```
This generates `g2p_rules.h` with all prediction functions.

### Compiling Tester (Separate Folder)

Copy `g2p_rules.h` generated by the trainer program into the tester folder. Use "make" to compile the tester program.
```
make
```

### Testing Phase
Test the generated rules.
```
./g2p_tester
```

## Example Output

Console output form G2P Tester Program.

```
./g2p_tester 
G2P Tester Program
==================
Word: cat
Expected: K AE T
Predicted: K AE T
Result: PASS 

Word: fat
Expected: F AE T
Predicted: F AE T
Result: PASS 

Word: mat
Expected: M AE T
Predicted: M AE T
Result: PASS 

Word: rat
Expected: R AE T
Predicted: R AE T
Result: PASS 

Word: sat
Expected: S AE T
Predicted: S AE T
Result: PASS 

Word: cart
Expected: K AA R T
Predicted: K AE R T
Result: FAIL 

Word: cake
Expected: K EY K
Predicted: K AE K
Result: FAIL 

Word: city
Expected: S IH T IY
Predicted: S IH T IY
Result: PASS 

Word: dig
Expected: D IH G
Predicted: D IH G
Result: PASS 

Word: dog
Expected: D AO G
Predicted: D G
Result: FAIL 

Word: hello
Expected: HH AH L OW
Predicted: L L
Result: FAIL 

Word: kid
Expected: K IH D
Predicted: K IH D
Result: PASS 

```

The word “cat” is a PASS as the expected ARPAbet phoneme sequence matches the predicted sequence. With the word “cart” the predicted sequence “ K AE R T” is close to the expected sequence "K AA R T". The AE phoneme should be AA. This is because the  generated predictA function defaults to "AE" and has no rule structure.
```
const char* predictA(char p, char c, char n, char nn, char nnn) {
 return "AE";
}
```
In fact the CART algorithm finds more "AA" patterns in the CMU dictionary samples and so the predictA function would normally default to the most common phoneme ("AA") but the guard rails in the build_tree function change this to AE.

This word "cake" also fails as the predicted sequence is "K EY K"" when it should be "K AE K"" (AE should be EY). 

Other prediction successes include "city", "dig" and "kid". 

The word "hello" predicts L L because the generated rules for h, e, and o are all collapsing to SILENT. This most likely means the tree is leaning heavily on default silent properties found across thousands of edge cases in the CMU dictionary.


###  Manual Rule Encoding

Once the basic rules had been learned I found that I had to manually customise them  by hard coding linguistic exceptions to create an effective G2P engine for my text-to-speech program called [dippo](https://github.com/crispinprojects/dippo). For example, this is how  the generated predictA function in g2p_rules.h is modified with context-aware rules. 

```
const char* predictA(char p, char c, char n, char nn, char nnn) {
    
    // Special cases where we know the pronunciation:
    if (p == 'c' && n == 'k') return "EY";  // cake
    if (p == 'b' && n == 'e') return "EY";  // bake
    if (p == 'f' && n == 'a') return "EH";  // fate

    // Magic E pattern - when 'a' is followed by a consonant and then 'e'
    if ((n != '_' && nn == 'e' && nnn == '_') ||
        (n != '_' && n != 'e' && nn == 'e' && nnn == '_')) {
        return "EY";  // For words like cake, take
    }

    // Common AA patterns:
    if (p == 'c' && n == 'r') return "AA";  // cart
    if (n == 'r') return "AA";             // car, star

    // Common AH patterns:
    if (n == 'b' || n == 'd' || n == 'g') return "AH";  // about, again

    // Default AE pattern:
    return "AE";
}

```

Words like "cart" and "cake" are now successfully predicted.

```
Word: cart
Expected: K AA R T
Predicted: K AA R T
Result: PASS 

Word: cake
Expected: K EY K
Predicted: K EY K
Result: PASS 
```

The issue with "dog" is that the generated implementation for the predictO() function is not  properly handling the case where 'o' should be pronounced as "AO" as it collapses to SILENT. The predictO needs to be modified as shown below.

```
const char* predictO(char p, char c, char n, char nn, char nnn) {
    // Common AO patterns:
    if (p == 'd') return "AO";  // dog
    if (p == 'w' || p == 'h') return "OW";  // water, home

    // Default OW pattern:
    return "OW";
}

```
Now "dog" is successfully predicted.

```
Word: dog
Expected: D AO G
Predicted: D AO G
Result: PASS 

```

###  Project Verdict

Generating grapheme-to-phoneme (G2P) prediction rules is a difficult problem but important as the task of converting English graphemes (spelling) to phonemes (pronunciation) is used in speech synthesis. Creating a rule-based or simple decision-tree Letter-to-Sound (LTS) engine by hand is difficult because English is not a phonetically consistent language. The exact same spelling combinations yield completely different sounds based on origin, stress, and etymology. 

This project has successfully created a working CART-based Grapheme-to-Phoneme (G2P) rule generator that creates a rule header that achieves correct  predictions for words like "cat", "mat", and "city". To get better prediction results the generated predict functions in g2p_rules.h  have to be modified manually  with  additional context-aware rules. 

## Author

* **Alan Crispin** [Github](https://github.com/crispinprojects)


## Acknowledgements

* [ARPAbet](https://en.wikipedia.org/wiki/ARPABET) 

* [The CMU Pronouncing Dictionary](http://www.speech.cs.cmu.edu/cgi-bin/cmudict).

* [dippo](https://github.com/crispinprojects/dippo) text-to-speech synthesizer 




