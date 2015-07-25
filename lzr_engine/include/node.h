
#include <vector>
#include <frame.h>

class Node
{
    public:
        void render();
        void link_input(Node* node);
        void unlink_input(Node* node);
    private:
        std::vector<Node*> inputs
};

