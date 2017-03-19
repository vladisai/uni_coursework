#include <bits/stdc++.h>

using namespace std;

const int THREADS = 4;

wstring& normalize(wstring& s) // assuming that words begin with letters
{
    while (s.size() > 0 && !isalpha(s.back()))
        s.pop_back();
    for (auto &c : s)
        c = tolower(c);
    return s;
}

int calculate(string file, wstring word)
{
    locale loc("pl_PL.UTF-8");
    wfstream in(file, ios::in);
    in.imbue(loc);
    int count = 0;
    wstring currentWord;
    while (in >> currentWord) {
        if (normalize(currentWord) == word)
            count++;
    }
    return count;
}

void calculate(wstring word, vector<string>::iterator begin, vector<string>::iterator end, promise<int>& p)
{
    int count = 0;
    for (auto it = begin; it != end; it++) {
        count += calculate(*it, word);
    }
    p.set_value(count);
}

int main()
{
    freopen("input.txt", "r", stdin);
    locale loc("pl_PL.UTF-8");
    wcout.imbue(loc);
    wcin.imbue(loc);

    wstring word;
    int filesCount;
    wcin >> word >> filesCount;
    vector<string> files(filesCount);
    normalize(word);

    int naive = 0;
    wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

    for (int i = 0; i < filesCount; i++) {
        wstring file;
        wcin >> file;
        files[i] = converter.to_bytes(file);
        naive += calculate(files[i], word);
    }

    cerr << naive << endl
         << endl;

    int filesPerProcess = filesCount / THREADS;
    int remainder = filesCount % THREADS;

    auto currentFile = files.begin();
    vector<promise<int> > promises(THREADS);
    vector<future<int> > futures(THREADS);

    for (int i = 0; i < THREADS; i++) {
        auto lastFile = currentFile + filesPerProcess;
        if (remainder-- > 0)
            lastFile++;

        thread t([&promises, &word, i, currentFile, lastFile]() { calculate(word, currentFile, lastFile, promises[i]); });
        t.detach();

        currentFile = lastFile;
    }

    int sum = 0;
    for (int i = 0; i < THREADS; i++) {
        sum += promises[i].get_future().get();
    }

    wcout << "In the files provided there were " << sum << " occurences of word " << word << endl;
    return 0;
}
