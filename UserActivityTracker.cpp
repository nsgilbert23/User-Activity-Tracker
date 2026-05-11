// User Activity Tracker - BST Implementation
// COE2265 Data Structures Project - Group 5

#include <iostream>
#include <string>
using namespace std;

struct Activity {
    long long timestamp;
    string user;
    string type;
    string details;
};

struct Node {
    Activity data;
    Node* left;
    Node* right;
};

class ActivityBST {
    Node* root;

    Node* insertNode(Node* node, Activity a) {
        if (node == nullptr) {
            Node* newNode = new Node;
            newNode->data = a;
            newNode->left = nullptr;
            newNode->right = nullptr;
            return newNode;
        }

        if (a.timestamp < node->data.timestamp)
            node->left = insertNode(node->left, a);
        else if (a.timestamp > node->data.timestamp)
            node->right = insertNode(node->right, a);

        return node;
    }

    Node* searchNode(Node* node, long long ts) {
        if (node == nullptr || node->data.timestamp == ts)
            return node;

        if (ts < node->data.timestamp)
            return searchNode(node->left, ts);
        else
            return searchNode(node->right, ts);
    }

    Node* findMin(Node* node) {
        while (node->left != nullptr)
            node = node->left;
        return node;
    }

    Node* deleteNode(Node* node, long long ts) {
        if (node == nullptr) return nullptr;

        if (ts < node->data.timestamp)
            node->left = deleteNode(node->left, ts);
        else if (ts > node->data.timestamp)
            node->right = deleteNode(node->right, ts);
        else {
            // node found - handle the three cases
            if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                return temp;
            }
            if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return temp;
            }
            // two children: replace with in-order successor
            Node* succ = findMin(node->right);
            node->data = succ->data;
            node->right = deleteNode(node->right, succ->data.timestamp);
        }
        return node;
    }

    // in-order traversal gives activities in ascending order of timestamp
    void inorder(Node* node) {
        if (node == nullptr) return;
        inorder(node->left);
        cout << node->data.timestamp << " | "
             << node->data.user << " | "
             << node->data.type << " | "
             << node->data.details << endl;
        inorder(node->right);
    }

    int countNodes(Node* node) {
        if (node == nullptr) return 0;
        return 1 + countNodes(node->left) + countNodes(node->right);
    }

public:
    ActivityBST() {
        root = nullptr;
    }

    bool insert(long long ts, string user, string type, string details) {
        if (searchNode(root, ts) != nullptr) return false;

        Activity a;
        a.timestamp = ts;
        a.user = user;
        a.type = type;
        a.details = details;
        root = insertNode(root, a);
        return true;
    }

    void search(long long ts) {
        Node* found = searchNode(root, ts);
        if (found != nullptr) {
            cout << "Activity found at timestamp " << ts << ":" << endl;
            cout << "  User: " << found->data.user << endl;
            cout << "  Type: " << found->data.type << endl;
            cout << "  Details: " << found->data.details << endl;
        } else {
            cout << "No activity found at timestamp " << ts << endl;
        }
    }

    bool update(long long ts, string user, string type, string details) {
        Node* found = searchNode(root, ts);
        if (found == nullptr) return false;
        found->data.user = user;
        found->data.type = type;
        found->data.details = details;
        return true;
    }

    bool remove(long long ts) {
        if (searchNode(root, ts) == nullptr) return false;
        root = deleteNode(root, ts);
        return true;
    }

    void displayAll() {
        if (root == nullptr) {
            cout << "No activities recorded." << endl;
            return;
        }
        cout << "Activity History (oldest to newest):" << endl;
        cout << "Timestamp | User | Type | Details" << endl;
        cout << "-----------------------------------" << endl;
        inorder(root);
    }

    int count() {
        return countNodes(root);
    }
};

void showMenu() {
    cout << "\n===== User Activity Tracker =====" << endl;
    cout << "1. Add activity" << endl;
    cout << "2. Search activity by timestamp" << endl;
    cout << "3. Update activity" << endl;
    cout << "4. Delete activity" << endl;
    cout << "5. Display all activities" << endl;
    cout << "6. Count total activities" << endl;
    cout << "0. Exit" << endl;
    cout << "Enter your choice: ";
}

int main() {
    ActivityBST tracker;

    // some sample data to start with
    tracker.insert(1715000000, "alice", "LOGIN", "from IP 196.49.10.22");
    tracker.insert(1714900000, "bob", "UPLOAD", "report.pdf");
    tracker.insert(1715100000, "carol", "DOWNLOAD", "lecture_notes.zip");
    tracker.insert(1714800000, "dan", "LOGIN", "from IP 41.74.55.1");
    tracker.insert(1715050000, "alice", "UPLOAD", "assignment3.cpp");

    cout << "Welcome to the User Activity Tracker." << endl;
    cout << "5 sample activities have been loaded for demonstration." << endl;

    int choice;
    long long ts;
    string user, type, details;

    while (true) {
        showMenu();
        cin >> choice;
        cin.ignore();

        if (choice == 0) {
            cout << "Exiting program. Goodbye!" << endl;
            break;
        }
        else if (choice == 1) {
            cout << "Enter timestamp (number): ";
            cin >> ts;
            cin.ignore();
            cout << "Enter user: ";
            getline(cin, user);
            cout << "Enter activity type (LOGIN/UPLOAD/DOWNLOAD): ";
            getline(cin, type);
            cout << "Enter details: ";
            getline(cin, details);

            if (tracker.insert(ts, user, type, details))
                cout << "Activity added successfully." << endl;
            else
                cout << "An activity already exists at that timestamp." << endl;
        }
        else if (choice == 2) {
            cout << "Enter timestamp to search: ";
            cin >> ts;
            cin.ignore();
            tracker.search(ts);
        }
        else if (choice == 3) {
            cout << "Enter timestamp of activity to update: ";
            cin >> ts;
            cin.ignore();
            cout << "Enter new user: ";
            getline(cin, user);
            cout << "Enter new activity type: ";
            getline(cin, type);
            cout << "Enter new details: ";
            getline(cin, details);

            if (tracker.update(ts, user, type, details))
                cout << "Activity updated successfully." << endl;
            else
                cout << "No activity found at that timestamp." << endl;
        }
        else if (choice == 4) {
            cout << "Enter timestamp of activity to delete: ";
            cin >> ts;
            cin.ignore();

            if (tracker.remove(ts))
                cout << "Activity deleted successfully." << endl;
            else
                cout << "No activity found at that timestamp." << endl;
        }
        else if (choice == 5) {
            tracker.displayAll();
        }
        else if (choice == 6) {
            cout << "Total activities: " << tracker.count() << endl;
        }
        else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
