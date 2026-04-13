/*
 * ============================================================
 *   ONLINE VOTING SYSTEM — C Programming Project
 *   Console-based | Authentication | Duplicate prevention
 * ============================================================
 *
 *  HOW TO COMPILE & RUN:
 *    gcc -Wall -o voting_system voting_system.c
 *    ./voting_system
 *
 *  PRE-REGISTERED VOTER IDs:  V001 – V020
 *  ADMIN PASSWORD (reset):    admin123
 * ============================================================
 */

#include <stdio.h>
#include <string.h>

/* ─── Constants ──────────────────────────────────────────── */
#define MAX_VOTERS     20
#define MAX_CANDIDATES  5
#define ID_LEN         10

/* ─── Structures ─────────────────────────────────────────── */
typedef struct {
    char id[ID_LEN];
    int  hasVoted;        /* 0 = not voted, 1 = voted        */
} Voter;

typedef struct {
    int  number;
    char name[40];
    char party[30];
    int  votes;
} Candidate;

/* ─── Global data ────────────────────────────────────────── */
Voter voters[MAX_VOTERS] = {
    {"V001",0},{"V002",0},{"V003",0},{"V004",0},{"V005",0},
    {"V006",0},{"V007",0},{"V008",0},{"V009",0},{"V010",0},
    {"V011",0},{"V012",0},{"V013",0},{"V014",0},{"V015",0},
    {"V016",0},{"V017",0},{"V018",0},{"V019",0},{"V020",0}
};
int totalVoters = 20;

Candidate candidates[MAX_CANDIDATES] = {
    {1, "Alice Johnson",  "Progressive Party",  0},
    {2, "Bob Martinez",   "Liberty Alliance",   0},
    {3, "Carol Singh",    "United Front",       0},
    {4, "David Chen",     "Reform Movement",    0},
    {5, "Eva Williams",   "Green Future",       0}
};
int totalCandidates = 5;

/* ─── Helpers ────────────────────────────────────────────── */
void printLine(char ch, int len) {
    for (int i = 0; i < len; i++) putchar(ch);
    putchar('\n');
}

void showBanner(void) {
    printLine('=', 55);
    printf("           ONLINE VOTING SYSTEM\n");
    printLine('=', 55);
}

/* Returns voter array index, or -1 if not found */
int authenticateVoter(const char *id) {
    for (int i = 0; i < totalVoters; i++)
        if (strcmp(voters[i].id, id) == 0)
            return i;
    return -1;
}

/* ─── Display candidate list ─────────────────────────────── */
void showCandidates(void) {
    printf("\n  %-4s  %-22s  %s\n", "No.", "Candidate", "Party");
    printLine('-', 52);
    for (int i = 0; i < totalCandidates; i++)
        printf("  [%d]  %-22s  %s\n",
               candidates[i].number,
               candidates[i].name,
               candidates[i].party);
    printLine('-', 52);
}

/* ─── Cast a vote ────────────────────────────────────────── */
void castVote(int voterIdx) {
    int choice;
    showCandidates();
    printf("\n  Enter candidate number (1-%d): ", totalCandidates);

    if (scanf("%d", &choice) != 1
        || choice < 1
        || choice > totalCandidates) {
        while (getchar() != '\n');          /* flush bad input */
        printf("\n  [ERROR] Invalid choice. Vote NOT recorded.\n");
        return;
    }

    candidates[choice - 1].votes++;
    voters[voterIdx].hasVoted = 1;
    printf("\n  [SUCCESS] Vote cast for %s!\n",
           candidates[choice - 1].name);
}

/* ─── Show election results ──────────────────────────────── */
void showResults(void) {
    int totalCast = 0, winnerIdx = 0;

    for (int i = 0; i < totalCandidates; i++) {
        totalCast += candidates[i].votes;
        if (candidates[i].votes > candidates[winnerIdx].votes)
            winnerIdx = i;
    }

    printLine('=', 55);
    printf("               ELECTION RESULTS\n");
    printLine('=', 55);
    printf("  %-22s  %-8s  %s\n", "Candidate", "Votes", "Bar");
    printLine('-', 55);

    for (int i = 0; i < totalCandidates; i++) {
        /* Scale bar to 20 chars */
        int bars = (totalCast > 0)
                   ? (candidates[i].votes * 20) / totalCast : 0;
        printf("  %-22s  %-8d  [",
               candidates[i].name, candidates[i].votes);
        for (int b = 0; b < bars;  b++) putchar('#');
        for (int b = bars; b < 20; b++) putchar(' ');
        printf("]\n");
    }

    printLine('=', 55);
    printf("  Total votes cast : %d / %d\n", totalCast, totalVoters);
    printLine('-', 55);

    if (totalCast == 0) {
        printf("  No votes have been cast yet.\n");
    } else {
        /* Detect tie */
        int tied = 0;
        for (int i = 0; i < totalCandidates; i++)
            if (i != winnerIdx &&
                candidates[i].votes == candidates[winnerIdx].votes)
                tied = 1;

        if (tied)
            printf("  RESULT : It's a TIE — no single winner!\n");
        else
            printf("  WINNER : %s (%s) with %d votes!\n",
                   candidates[winnerIdx].name,
                   candidates[winnerIdx].party,
                   candidates[winnerIdx].votes);
    }
    printLine('=', 55);
}

/* ─── Admin: reset all data ──────────────────────────────── */
void resetVotes(void) {
    for (int i = 0; i < totalVoters;     i++) voters[i].hasVoted  = 0;
    for (int i = 0; i < totalCandidates; i++) candidates[i].votes = 0;
    printf("\n  [SUCCESS] All votes have been reset.\n");
}

/* ─── Main menu loop ─────────────────────────────────────── */
void mainMenu(void) {
    int  option;
    char voterID[ID_LEN + 5];

    while (1) {
        showBanner();
        printf("  1. Cast Your Vote\n");
        printf("  2. View Results\n");
        printf("  3. Reset Votes  [Admin]\n");
        printf("  4. Exit\n");
        printLine('-', 55);
        printf("  Choose an option: ");

        if (scanf("%d", &option) != 1) {
            while (getchar() != '\n');
            continue;
        }

        switch (option) {

        /* ── 1. Vote ── */
        case 1:
            printf("\n  Enter Voter ID (e.g. V001): ");
            scanf("%9s", voterID);

            /* Accept lowercase input */
            for (int i = 0; voterID[i]; i++)
                if (voterID[i] >= 'a' && voterID[i] <= 'z')
                    voterID[i] -= 32;

            {
                int idx = authenticateVoter(voterID);
                if (idx == -1)
                    printf("\n  [ERROR] Voter ID '%s' not found.\n", voterID);
                else if (voters[idx].hasVoted)
                    printf("\n  [ERROR] %s has already voted.\n", voterID);
                else {
                    printf("\n  [OK] Authenticated. Welcome, %s!\n", voterID);
                    castVote(idx);
                }
            }
            printf("\n  Press ENTER to continue...");
            while (getchar() != '\n'); getchar();
            break;

        /* ── 2. Results ── */
        case 2:
            showResults();
            printf("\n  Press ENTER to continue...");
            while (getchar() != '\n'); getchar();
            break;

        /* ── 3. Admin reset ── */
        case 3: {
            char pwd[20];
            printf("\n  Enter admin password: ");
            scanf("%19s", pwd);
            if (strcmp(pwd, "admin123") == 0)
                resetVotes();
            else
                printf("\n  [ERROR] Wrong password.\n");
            printf("\n  Press ENTER to continue...");
            while (getchar() != '\n'); getchar();
            break;
        }

        /* ── 4. Exit ── */
        case 4:
            printLine('=', 55);
            printf("  Thank you for using the Voting System. Goodbye!\n");
            printLine('=', 55);
            return;

        default:
            printf("\n  [ERROR] Invalid option. Try 1-4.\n\n");
        }
    }
}

/* ─── Entry point ────────────────────────────────────────── */
int main(void) {
    mainMenu();
    return 0;
}