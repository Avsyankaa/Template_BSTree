#include <fstream>
#include <string>
#include <iostream>
#include <initializer_list>
#include <cstddef>
namespace BSTree {
    template <typename T>
    struct Node {
        T data;
        Node<T>* left;
        Node<T>* right;
        Node(T val);
    };
    enum class traversal_order {pre, in, post};
    template <typename T>
    class Tree {
        Node<T>* root;
        auto print_elements(Node<T>* curr, int space) const -> void;
        auto deleting(Node<T>*&curr) -> void;
        auto straight_detour(Node<T> *curr) const -> void;
        auto back_detour(Node<T> *curr) const -> void;
        auto transverse_detour(Node<T> *curr) const -> void;
        auto save_tree_to_the_file_recursion (Node<T> * curr, int  space, std::ostream &File) ->void;
        auto save_tree_to_the_file_straight_detour (Node<T> * curr, std::ostream &File) -> void;
        auto copy (Node<T> * curr) -> void;
        auto for_operator (std::ostream& stream, Node<T> * curr) -> void;

        public:
        Tree();
        Tree(std::initializer_list<T> list);
        Tree(const Tree& tree);
        auto insert(T value) -> bool;
        auto print() const -> void;
        auto empty() const -> bool;
        auto add_node (T value) -> void;
        auto remove (T value) -> bool;
        auto save (const std::string& path) -> bool ;
        auto load (const std::string& path)->bool;
        auto exists (T value) const -> bool;
        auto print(traversal_order order) const -> void;
        auto operator=(const Tree&) -> Tree&;
        ~Tree();
        template <typename T1>
        auto friend operator<<(std::ostream& stream, Tree<T1>& tree) -> std::ostream&
        {
            tree.for_operator (stream, tree.root);
            return stream;
        }
    };
    template <typename T>
    Tree<T>:: Tree ()
    :root {nullptr}
    {}
    template <typename T>
    Node<T>:: Node (T val): data {val}, right {nullptr}, left {nullptr} {
    }
    template <typename T>
    auto Tree<T>:: empty ( ) const  -> bool {
        return (root==nullptr? true : false);
    }
    template <typename T>
    Tree<T>:: Tree (std::initializer_list<T> list) {
        root=nullptr;
        for (auto value:list) {
            insert(value);
        }
    }
    template <typename T>
    Tree<T>:: Tree (const Tree<T>& tree) {
        this->root=nullptr;
        copy(tree.root);
    }
    template <typename T>
    auto Tree<T>:: copy (Node<T> * curr) -> void {
        insert(curr->data);
        if (curr->left!=nullptr) copy (curr->left);
        if (curr->right!=nullptr) copy (curr->right);
    }
    template <typename T>
    auto Tree<T>:: insert (T value) -> bool {
        if (root == nullptr) {
            root = new Node<T> {value};
            return true;
        }
        Node<T> * parent= root;
        Node<T> * curr= root;
        while ( curr != nullptr) {
            if (curr->data==value) return false;
            if (curr->data < value) curr= curr->right;
            else curr= curr->left;
            if (curr!=nullptr) parent= curr;
        }
        if (parent->data< value) parent->right = new Node <T> {value};
        else parent->left = new Node<T> {value};
        return true;
    }
    template <typename T>
    auto Tree<T>:: print_elements (Node<T> * curr, int  space) const ->void {
        if (curr!=nullptr) {
            if (curr->right!=nullptr) print_elements (curr->right, space+1);
            for (int i=0; i< space; i++) {
                std::cout<< "   ";
            }
            if ((curr->data)!=(root->data))
                std::cout<< "--";
            std::cout<< curr->data << std::endl;
            if (curr->left!=nullptr)  print_elements (curr->left, space+1);
        }
        else std::cout<<"Tree is empty"<< std::endl;
    }
    template <typename T>
    auto Tree<T>:: print () const -> void {
        print_elements (root, 0);
    }
    template <typename T>
    auto Tree<T>:: deleting (Node<T> *& curr) -> void {
        if (curr==nullptr) return;
        if (curr->right!=nullptr)
            deleting (curr->right);
        if (curr-> left!= nullptr) deleting (curr->left);
        delete curr;
        curr = nullptr;
    }
    template <typename T>
    auto Tree<T>:: straight_detour (Node<T> * curr) const -> void {
        std::cout<< curr->data<< " ";
        if (curr->left!=nullptr) straight_detour (curr->left);
        if (curr->right!=nullptr) straight_detour (curr->right);
    }
    template <typename T>
    auto Tree<T>:: back_detour (Node<T> * curr) const -> void {
        if (curr->left!=nullptr) back_detour (curr->left);
        if (curr->right!=nullptr) back_detour (curr->right);
        std::cout<< curr->data<<" ";
    }
    template <typename T>
    auto Tree<T>:: transverse_detour (Node<T> * curr) const -> void {
        if (curr->left!=nullptr) transverse_detour (curr->left);
        std::cout<< curr->data<<" ";
        if (curr->right!=nullptr) transverse_detour (curr->right);
    }
    template <typename T>
    auto Tree<T>:: print(traversal_order order) const ->void {

        switch (order) {
        case traversal_order::pre :
            straight_detour(root);
            break;
        case traversal_order::in :
            transverse_detour(root);
            break;
        case traversal_order::post:
            back_detour(root);
            break;
        }
    }
    template <typename T>
    Tree<T>:: ~Tree() {
        deleting (root);
    }
    template <typename T>
    auto Tree<T>:: add_node (T value) -> void {
        if (!(this->insert (value)))
        {
            std::cerr << "This Node is already exist!!!" << std::endl;
        }
    }
    template <typename T>
    auto Tree<T>:: remove (T value) -> bool {
        if ((root->right==nullptr)&& (root->left==nullptr) && (root->data==value)) {
            delete root;
            root= nullptr;
            return true;
        }
        Node<T> * parent= root;
        Node<T> * curr= root;
        Node<T> * parent_del =root;
        int value_new;
        while (curr!=nullptr) {
            if (curr->data==value) break;
            parent_del= curr;
            if (curr->data> value) curr= curr->left;
            else {
                if (curr->data< value) curr= curr->right;
            }
        } // нашли удаляемый узел и его родителя
        if (curr==nullptr)
            return false; //узел не существует в дереве
        if (curr->left!=nullptr) {
            parent = curr;
            curr= curr->left;
            while (1) {
                if (curr->right != nullptr) {
                    parent= curr;
                    curr = curr->right;
                }
                if (curr->right==nullptr)  break;
            }
            value_new= curr->data;
            if ((parent->left!=nullptr) && (parent->left->data==value_new)) {
                delete parent->left;
                parent->left=nullptr;
            }
            else {
                delete parent->right;
                parent->right= nullptr;
            }
        }
        else {
            if (curr->right!=nullptr) {
                parent = curr;
                curr= curr->right;
                while (1) {
                    if (curr->left != nullptr) {
                        parent= curr;
                        curr = curr->left;
                    }
                    if (curr->left==nullptr)  break;
                }
                value_new= curr->data;
                if ((parent->right!=nullptr) && (parent->right->data==value_new)) {
                    delete parent->right;
                    parent->right=nullptr;
                }
                else {
                    delete parent->left;
                    parent->left= nullptr;
                }
            }
            else {
                if (parent_del->data> value) {
                    delete parent_del->left;
                    parent_del->left=nullptr;
                    return true;
                }
                delete parent_del->right;
                parent_del->right=nullptr;
                return true;
            }
        }
        if (parent_del->data> value)
            parent_del->left->data = value_new;
        if (parent_del->data < value)
            parent_del->right->data = value_new;
        if (parent_del->data==value)
            root->data= value_new;
        return true;
    }
    template <typename T>
    auto Tree<T>:: save_tree_to_the_file_recursion (Node<T> * curr, int  space, std::ostream &File) ->void {
        if (curr!=nullptr) {
            if (curr->right!=nullptr)  save_tree_to_the_file_recursion (curr->right, space+1, File);
            for (int i=0; i< space; i++) {
                File<< "   ";
            }
            if ((curr->data)!=(root->data))
                File<< "--";
            File<< curr->data << std::endl;
            if (curr->left!=nullptr)  save_tree_to_the_file_recursion (curr->left, space+1, File);
        }
    }
    template <typename T>
    auto Tree<T>:: save_tree_to_the_file_straight_detour (Node<T> * curr, std::ostream &File) -> void {
        File<< curr->data<< " ";
        if (curr->left!=nullptr) save_tree_to_the_file_straight_detour (curr->left,File);
        if (curr->right!=nullptr) save_tree_to_the_file_straight_detour (curr->right,File);
    }
    template <typename T>
    auto Tree<T>:: save (const std::string& path) -> bool {
        std::ifstream File1(path) ;
        long file_size;
        File1.seekg(0, std::ios::end);
        file_size = File1.tellg();
        File1.close();
        std::ofstream File(path);
        std::string decision ="yes";
        if ((file_size!=-1) && (file_size!=0))  {
            std::cout << "Do you want to rewrite file? (yes/ no)" << std::endl;
            std::cin >> decision;
        }
        File.close();
        File.open (path);
        if ((decision == "y") || (decision == "yes") || (decision == "Y") ||
                (decision == "Yes") || (decision == "YES"))
        {
            save_tree_to_the_file_straight_detour (root,File);
            File<< std::endl;
            save_tree_to_the_file_recursion (root, 0, File);
        }
        File.close();
        return true;
    }
    template <typename T>
    auto Tree<T>:: load (const std::string& path)->bool {
        std::ifstream File(path);
        if (!File.is_open()) return false;
        std::string tree_string;
        getline (File,tree_string);
        int elements_count=0;
        for (int i=0; i< tree_string.length(); i++) {
            if (tree_string[i]==' ') elements_count++;
        }
        File.close();
        File.open(path);
        for (int i=0; i<elements_count; i++) {
            File >> tree_string;
            this->insert (atoi( tree_string.c_str()));
        }
        File.close();
        return true;
    }
    template <typename T>
    auto Tree<T>:: exists (T value) const -> bool {
        Node<T> * curr = root;
        while (curr!=nullptr) {
            if (curr->data==value) return true;
            if (curr->data > value) curr=curr->left;
            else curr=curr->right;
        }
        return false;
    }
    template <typename T>
    auto Tree<T>::for_operator (std::ostream& stream, Node<T> * curr) -> void {
        stream<< curr->data<< " ";
        if (curr->left!=nullptr) for_operator (stream, curr->left);
        if (curr->right!=nullptr) for_operator (stream, curr->right);
    }
    template <typename T>
    auto Tree<T>::operator=(const Tree<T>& tree) -> Tree& {
        deleting(this->root);
        copy(tree.root);
        return *this;
    }
}
