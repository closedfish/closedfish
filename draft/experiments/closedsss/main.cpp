#include "../../../lib/board_implementation/CFBoard.cpp"
#include "../../../lib/board_implementation/CFBoard.h"
#include "../../../lib/board_implementation/naiveCheckCheck.cpp"
#include <string>
#include <tuple>
#include <unordered_set>
#include <vector>

/**
 * @brief Hash the board to avoid repetition
 *
 * @param board
 * @return uint64_t
 */
std::string hash_board(CFBoard board) { return board.toFEN(); }

class SearchTree {
  private:
    std::unordered_set<std::string> hashes;
    class SearchTreeNode {
      private:
        CFBoard board;
        std::vector<SearchTreeNode *> children;

      public:
        int depth;
        int height;
        int size;
        SearchTreeNode(const CFBoard &board, int d = 0)
            : board(board), depth(d), height(0), size(1) {}
        /**
         * @brief Depth-first search from the current position.
         *
         * @param steps_limit limit the number of steps
         * @return int the number of leaf nodes (i.e. breakthrough)
         */
        int search(int &steps_limit, std::unordered_set<std::string> &hashes) {
            // std::cout << "DBG " << steps_limit << std::endl;
            if (steps_limit <= 0)
                return -1;
            if (hashes.find(hash_board(board)) != hashes.end())
                return 0;
            hashes.insert(hash_board(board));
            int leaves = 0;
            std::vector<CFBoard> next_boards; // assume we have this
            // Sirawit: in this experiment, we don't care about the depth
            for (int i = 0; i < 64; i++) {
                // checking every square for our pieces
                int k = board.getPieceFromCoords(i);
                if (k < 0 || k % 2)
                    continue; // opponent's turn: don't care
                uint64_t next_positions = board.getLegalMoves(k, i);
                while (next_positions) {
                    int j = __builtin_ctzll(next_positions & -next_positions);
                    next_positions ^= 1ll << (uint64_t)j;
                    next_boards.push_back(board);
                    // std::cout << "Trying to move from " << i << " to " << j
                    //           << std::endl;
                    next_boards.back().removePiece(i);
                    if (next_boards.back().getPieceFromCoords(j) != -1) {
                        size++;
                        leaves++;
                        next_boards.pop_back();
                    } else {
                        next_boards.back().addPiece(k, j);
                    }
                    // std::cout << "DONE" << std::endl;
                }
            }
            // END
            for (const CFBoard &each : next_boards) {
                children.push_back(new SearchTreeNode(each, depth + 1));
                size++;
            }
            for (SearchTreeNode *each : children) {
                size--;
                int current_leaves = each->search(--steps_limit, hashes);
                if (current_leaves < 0)
                    leaves = -1;
                else
                    leaves += current_leaves;
                size += each->size;
                height = std::max(height, each->height + 1);
            }
            return leaves;
        }
    };
    SearchTreeNode *root = nullptr;

  public:
    SearchTree(CFBoard init) { root = new SearchTreeNode(init); }
    /**
     * @brief Searches the tree
     *
     * @param steps_limit limit of the number of possible edge traversals
     * @return std::tuple<int,int,int> (height of root, size of the tree, number
     * of leaves)
     */
    std::tuple<int, int, int> search(int steps_limit = 100000) {
        int leaves = root->search(steps_limit, hashes);
        return std::make_tuple(root->height, root->size, leaves);
    }
};

int main() {
    // test on the initial default board
    CFBoard init("3k4/8/7p/2p1p1pP/1pPpPpP1/1P1P1P2/N7/2K5 w - - 0 1");
    SearchTree tree(init);
    int a, b, c;
    std::tie(a, b, c) = tree.search(80000);
    std::cout << a << " " << b << " " << c << std::endl;
    return 0;
}