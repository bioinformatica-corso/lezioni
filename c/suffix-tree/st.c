/*
  Copyright (C) 2014 by Gianluca Della Vedova


  You can redistribute this file and/or modify it
  under the terms of the GNU Lesser General Public License as published
  by the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Box is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this file
  If not, see <http://www.gnu.org/licenses/>.
*/

#include "st.h"

(st_node_t *)
st_new(void) {
        st_node_t * root = malloc(sizeof(st_node_t));
        memset(root, 0, sizeof(root));
        return root;
}

void
compute_string_depth(st_node_t* tree) {
        void _recursion_step(st_node_t* tree, unsigned int depth_parent) {
                tree->string_depth = depth_parent + tree->end_label - tree->begin_label;
                for (unsigned int i = 0; i < tree->children_size; i++)
                        _recursion_step(tree->children_list[tree->character_list[i]], tree->string_depth);
        }
        _recursion_step(tree, 0);
}
