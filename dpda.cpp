#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

using namespace std;

struct Row {
    int step;
    string state;
    string unread;
    string stack; // top -> bottom
    string delta;
    string action;
};

string stack_display(const vector<string>& st) {
    if (st.empty()) return "e";
    string res;
    for (int i = st.size() - 1; i >= 0; --i) res += st[i];
    return res;
}

vector<Row> run_dpda(const string &input) {
    vector<Row> trace;
    int step = 0;

    string state = "p";
    vector<string> stack;

    auto unread = [&](int pos){ return input.substr(pos); };

    trace.push_back({step++, state, unread(0), stack_display(stack), "-", "-"});

    // Step (1): (p, ε, ε) -> (q, S)
    state = "q";
    stack.push_back("S");
    trace.push_back({step++, state, unread(0), stack_display(stack),
                     "(p, ε, ε) -> (q, S)", "push(S)"});

    int pos = 0;

    while (true) {
        char la = (pos < (int)input.size()) ? input[pos] : '\0';
        string unread_str = (pos < (int)input.size()) ? input.substr(pos) : "";

        if (stack.empty()) {
            if (la == '$') {
                trace.push_back({step++, state, unread_str, "e",
                                 "(q, $, e) -> (q$, e)", "move->q$"});
                state = "q$";
                trace.push_back({step++, state, "", "e",
                                 "(q$, ε, e) -> (finalize)", "finalize"});
            } else {
                trace.push_back({step++, state, unread_str, "e",
                                 "(q, " + string(1, la) + ", e) -> reject", "reject"});
            }
            break;
        }

        string top = stack.back();

        // Expand S
        if (top == "S") {
            if (la == 'a') {
                trace.push_back({step++, state, unread_str, stack_display(stack),
                                 "(q, a, S) -> (q, a S b)", "S -> a S b"});
                stack.pop_back();
                stack.push_back("b");
                stack.push_back("S");
                stack.push_back("a");
                continue;
            } else { // b or $
                trace.push_back({step++, state, unread_str, stack_display(stack),
                                 "(q, " + string(1, la) + ", S) -> (q, ε)", "S -> ε (pop S)"});
                stack.pop_back();
                continue;
            }
        }

        // Consume terminals
        if (top == "a" && la == 'a') {
            trace.push_back({step++, state, unread_str, stack_display(stack),
                             "(q, a, a) -> (q, ε)", "read(a) & pop(a)"});
            stack.pop_back();
            pos++;
            continue;
        }

        if (top == "b" && la == 'b') {
            trace.push_back({step++, state, unread_str, stack_display(stack),
                             "(q, b, b) -> (q, ε)", "read(b) & pop(b)"});
            stack.pop_back();
            pos++;
            continue;
        }

        // Reject if no rules match
        trace.push_back({step++, state, unread_str, stack_display(stack),
                         "(no rule)", "reject"});
        break;
    }

    return trace;
}

void print_trace(const vector<Row>& t) {
    cout << left << setw(6) << "Step" << setw(10) << "State" << setw(14) << "Unread"
         << setw(20) << "Stack(top->bottom)" << setw(32) << "Δ Used" << "R Used\n";
    for (auto &r : t) {
        cout << left << setw(6) << r.step
             << setw(10) << r.state
             << setw(14) << (r.unread.empty() ? "e" : r.unread)
             << setw(20) << r.stack
             << setw(32) << r.delta
             << r.action << "\n";
    }
    cout << "\n";
}

int main() {
    vector<string> tests = {
        "aabb$",        // 1
        "aaabbb$",      // 2
        "aaaabbbb$",    // 3
        "aaaaabbbbb$",  // 4
        "aaaaaabbbbbb$" // 5
    };

    while (true) {
        cout << "Select an input to run (1-5), or 0 to quit:\n";
        cout << endl;
	cout << "1: aabb$\n2: aaabbb$\n3: aaaabbbb$\n4: aaaaabbbbb$\n5: aaaaaabbbbbb$\n";
        int choice;
	cout << endl;
        cin >> choice;

        if (choice == 0) {
            cout << "\nExiting program.\n";
            break;
        }

        if (choice < 1 || choice > 5) {
            cout << "Invalid choice. Try again.\n";
            continue;
        }

        string input = tests[choice - 1];
        cout << "===== Input: " << input << " =====\n";
        auto trace = run_dpda(input);
        print_trace(trace);
    }

    return 0;
}
