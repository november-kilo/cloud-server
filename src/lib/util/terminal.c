#define TERM_ESC "\033["

private string colorString(mixed a, mixed b, string str) {
    return TERM_ESC + a + "m" + str + TERM_ESC + b + "m";
}

static string bell(void) {
    return "\a";
}

static string clear(void) {
    return TERM_ESC + "2J";
}

static string bold(string str) {
    return colorString(1, 22, str);
}

static string dim(string str) {
    return colorString(2, 22, str);
}

static string inverse(string str) {
    return colorString(7, 27, str);
}

static string italic(string str) {
    return colorString(3, 23, str);
}

static string strikeThrough(string str) {
    return colorString(9, 29, str);
}

static string underline(string str) {
    return colorString(4, 24, str);
}

static string color256(int a, string str) {
    string aStr;

    if (a < 0 || a > 255) {
        error("Terminal: Invalid color");
    }

    aStr = (string) a;

    if (a < 10) {
        aStr = "00" + a;
    } else if (a < 100) {
        aStr = "0" + a;
    }

    return "\x1b[38;5;" + aStr + "m" + str + "\x1b[0m";
}

static string black(string str, varargs int useBold) {
    return colorString(useBold ? 90 : 30, 39, str);
}

static string blue(string str, varargs int useBold) {
    return colorString(useBold ? 94 : 34, 39, str);
}

static string cyan(string str, varargs int useBold) {
    return colorString(useBold ? 96 : 36, 39, str);
}

static string green(string str, varargs int useBold) {
    return colorString(useBold ? 92 : 32, 39, str);
}

static string magenta(string str, varargs int useBold) {
    return colorString(useBold ? 95 : 35, 39, str);
}

static string red(string str, varargs int useBold) {
    return colorString(useBold ? 91 : 31, 39, str);
}

static string yellow(string str, varargs int useBold) {
    return colorString(useBold ? 93 : 33, 39, str);
}

static string white(string str, varargs int useBold) {
    return colorString(useBold ? 97 : 37, 39, str);
}

static string gray(string str) {
    return colorString(90, 39, str);
}

static string grey(string str) {
    return gray(str);
}

private string move(int amount, string code) {
    return TERM_ESC + amount + code;
}

static string moveBack(int x) {
    return move(x, "D");
}

static string moveDown(int y) {
    return move(y, "B");
}

static string moveForward(int x) {
    return move(x, "C");
}

string moveUp(int y) {
    return move(y, "A");
}

static string arrowDown(void) {
    return "↓";
}

static string arrowLeft(void) {
    return "←";
}

static string arrowRight(void) {
    return "→";
}

static string arrowUp(void) {
    return "↑";
}

static string check(void) {
    return "✔";
}

static string cross(void) {
    return "✘";
}

static string eks(void) {
    return "✖";
}

static string info(void) {
    return "ℹ";
}

static string play(void) {
    return "▶";
}

static string hpipe(void) {
    return "─";
}

static string hpipeDouble(void) {
    return "═";
}

static string htpipe(void) {
    return "├";
}

static string upLpipe(varargs int reversed) {
    return reversed ? "┌" : "┐";
}

static string upLpipeRound(varargs int reversed) {
    return reversed ? "╭" : "╮";
}

static string upLpipeDouble(varargs int reversed) {
    return reversed ? "╔" : "╗";
}

static string lpipe(varargs int reversed) {
    return reversed ? "┘" : "└";
}

static string lpipeRound(varargs int reversed) {
    return reversed ? "╯" : "╰";
}

static string lpipeDouble(varargs int reversed) {
    return reversed ? "╝" : "╚";
}

static string tpipe(void) {
    return "┬";
}

static string vpipe(void) {
    return "│";
}

static string vpipeDouble(void) {
    return "║";
}

static string pointer(varargs int useLarge) {
    return useLarge ? "❯" : "›";
}

static string star(void) {
    return "★";
}

static string fourCorners(void) {
    return "⛶";
}

static string shrug(void) {
    return "¯\\_(ツ)_/¯";
}
