#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
using namespace std;

class sVoter {
public:
    int id;
    string name;
    int votes;

    sVoter(int id, string name) : id(id), name(name), votes(0) {}
};

class TreeNode {
public:
    sVoter* voter;
    TreeNode* left;
    TreeNode* right;

    TreeNode(sVoter* voter) : voter(voter), left(nullptr), right(nullptr) {}
};

class MaxHeap {
private:
    vector<sVoter*> heap;

    void heapify(int n, int i) {
        int largest = i;          // Initialize largest as root
        int left = 2 * i + 1;     // Left child index
        int right = 2 * i + 2;    // Right child index

        // Check if left child exists and is larger than root
        if (left < n && heap[left]->votes > heap[largest]->votes)
            largest = left;

        // Check if right child exists and is larger than largest so far
        if (right < n && heap[right]->votes > heap[largest]->votes)
            largest = right;

        // If largest is not root, swap and continue heapifying
        if (largest != i) {
            swap(heap[i], heap[largest]);
            heapify(n, largest);
        }
    }

public:
    void buildHeap(vector<sVoter*>& candidates) {
        heap = candidates;
        int n = heap.size();

        // Build the heap (start from last non-leaf node)
        for (int i = n / 2 - 1; i >= 0; i--) {
            heapify(n, i);
        }
    }

    void displayHeapSorted() {
        cout << "\nResults (Highest Votes First):\n";
        int n = heap.size();
        for (int i = n - 1; i >= 0; i--) {
            // Display the root of the heap (max element)
            cout << "Candidate ID: " << heap[0]->id
                 << ", Name: " << heap[0]->name
                 << ", Votes: " << heap[0]->votes << endl;

            // Move current root to end and reduce the heap size
            swap(heap[0], heap[i]);
            heapify(i, 0);
        }
    }
};

class sBST {
private:
    TreeNode* root;

    TreeNode* sinsert(TreeNode* node, sVoter* voter) {
        if (node == nullptr)
            return new TreeNode(voter);

        if (voter->id < node->voter->id)
            node->left = sinsert(node->left, voter);
        else if (voter->id > node->voter->id)
            node->right = sinsert(node->right, voter);

        return node;
    }

    sVoter* ssearch(TreeNode* node, int id) {
        if (node == nullptr || node->voter->id == id)
            return node ? node->voter : nullptr;

        if (id < node->voter->id)
            return ssearch(node->left, id);

        return ssearch(node->right, id);
    }

    void scollectVotes(TreeNode* node, vector<sVoter*>& candidates) {
        if (node == nullptr)
            return;

        candidates.push_back(node->voter);
        scollectVotes(node->left, candidates);
        scollectVotes(node->right, candidates);
    }

public:
    sBST() : root(nullptr) {}

    void saddVoter(int id, string name) {
        sVoter* voter = new sVoter(id, name);
        root = sinsert(root, voter);
    }

    sVoter* sfindVoter(int id) {
        return ssearch(root, id);
    }

    void svoteForCandidate(int id, unordered_set<int>& voters, int voterID) {
        if (voters.count(voterID)) {
            cout << "Fraud detected! Voter ID " << voterID << " has already voted.\n";
            return;
        }

        sVoter* candidate = sfindVoter(id);
        if (candidate) {
            candidate->votes++;
            voters.insert(voterID);
            cout << "Voted successfully for " << candidate->name << ". Total votes: " << candidate->votes << endl;
        } else {
            cout << "Candidate not found." << endl;
        }
    }

    void displayResultsMaxHeap() {
        vector<sVoter*> candidates;
        scollectVotes(root, candidates);

        MaxHeap heap;
        heap.buildHeap(candidates);
        heap.displayHeapSorted();
    }
};

int main() {
    sBST bst;
    unordered_set<int> voters; // To track who has voted
    int numCandidates;

    cout << "Enter the number of candidates to register: ";
    cin >> numCandidates;

    for (int i = 0; i < numCandidates; ++i) {
        int id;
        string name;
        cout << "Enter ID for candidate " << (i + 1) << ": ";
        cin >> id;
        cout << "Enter name for candidate " << (i + 1) << ": ";
        cin.ignore();
        getline(cin, name);
        bst.saddVoter(id, name);
    }

    int choice;
    do {
        cout << "\n-----------------------------\n";
        cout << "Voting Options:\n";
        cout << "1. Vote for an election\n";
        cout << "2. Show all votes (Max Heap)\n";
        cout << "0. Exit\n";
        cout << "-----------------------------\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int candidateID, voterID;
                cout << "Enter your voter ID: ";
                cin >> voterID;
                cout << "Enter candidate ID to vote: ";
                cin >> candidateID;
                bst.svoteForCandidate(candidateID, voters, voterID);
                break;
            }
            case 2:
                bst.displayResultsMaxHeap();
                break;
            case 0:
                cout << "Exiting.\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}