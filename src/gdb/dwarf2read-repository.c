/* -*- C -*-
 *  dwarf2read-repository.c
 *  gdb
 *
 *  Created by Eric Gallager on 5/10/19.
 *  Copyright 2019 George Washington University. All rights reserved.
 *  Originally split off from dwarf2read.c
 */

/* DWARF 2 debugging format support for GDB.

 Copyright 1994-2005, Free Software Foundation, Inc.

 Adapted by Gary Funck <gary@intrepid.com>, Intrepid Technology,
 Inc.  with support from Florida State University (under contract
 with the Ada Joint Program Office), and Silicon Graphics, Inc.
 Initial contribution by Brent Benson, Harris Computer Systems, Inc.,
 based on Fred Fish's (Cygnus Support) implementation of DWARF 1
 support in dwarfread.c

 This file is part of GDB.

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or (at
 your option) any later version.

 This program is distributed in the hope that it will be useful, but
 WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330,
 Boston, MA 02111-1307, USA.  */

#include "defs.h"
#include "dwarf2read.h"
#include "gdb_assert.h"
#include "elf/dwarf2.h"

/* APPLE LOCAL begin dwarf repository  */
/* NOTE:  Everything from here to the end of the file is APPLE LOCAL  */
/* ********************** REPOSITORY STUFF STARTS HERE ********************** */
/* DONE: moved to separate file (here, this one)
 This "section" contains several sub-sections:

 1. Red Black Trees.  This sub-section contains code for defining, creating
 and manipulating red-black trees, which is how we efficiently keep track of
 (and access) types already retrieved and decoded from a repository.

 2. Global repositories and data structures.  This sub-section contains code
 for tracking, controlling, and manipulating the dwarf types repositories.
 The code in this section is more high-level, dealing with stuff at the
 level of entire repositories.

 3. Accessing the sqlite3 database and decoding the dies.  This contains the
 low-level database access functions.  It includes the code for reading and
 decoding the dies, and translating the dwarf type information into gdb
 type structures.
 */

/* APPLE LOCAL begin red-black trees, part 2.  */
/* Begin repository sub-section 1: Red-black trees.  This section  implements
 the red-black tree algorithms from "Introduction to Algorithms" by Cormen,
 Leiserson, and Rivest.  A red-black tree is a 'semi-balanced' binary tree,
 where by semi-balanced it means that for any node in the tree, the height of
 one sub-tree is guaranteed to never be greater than twice the height of the
 other sub-tree.  Each node is colored either red or black, and a parent must
 never be the same color as its children.

 The following types, used by the functions in this section, are defined
 near the beginning of this file  (look for the label "red-black trees,
 part 1"):

 enum rb_tree_colors;  (type)
 struct rb_tree_node;  (type)

 This section defines the following functions:

 rb_tree_find_node (function)
 rb_tree_find_node_all_keys (function)
 rb_tree_find_and_remove_node (function)
 left_rotate       (function)
 right_rotate      (function)
 plain_tree_insert (function)
 rb_tree_insert    (function)
 rb_tree_remove_node (function)
 rb_tree_minimun (function)
 rb_tree_successor (function)
 rb_delete_fixup (function)
 rb_tree_remove_node (function)
 */

/* This function searches the tree ROOT recursively until it
 finds a node with the key KEY, which it returns.  If there
 is no such node in the tree it returns NULL.  */
struct rb_tree_node *
rb_tree_find_node(struct rb_tree_node *root, long long key, int secondary_key)
{
  if (!root)
    return NULL;

  if ((CORE_ADDR)key == root->key)
  {
    if (secondary_key < 0)
      return root;
    else if (secondary_key < root->secondary_key)
      return rb_tree_find_node (root->left, key, secondary_key);
    else
      return rb_tree_find_node (root->right, key, secondary_key);
  }
  else if ((CORE_ADDR)key < root->key)
    return rb_tree_find_node (root->left, key, secondary_key);
  else
    return rb_tree_find_node (root->right, key, secondary_key);
}


/* This function searches the tree ROOT recursively until it
 finds a node with the key KEY, secondary key SECONDARY_KEY and third key
 THIRD_KEY, which it returns.  If there is no such node in the tree it
 returns NULL.  */
struct rb_tree_node *
rb_tree_find_node_all_keys(struct rb_tree_node *root, long long key,
			   int secondary_key, long long third_key)
{
  if (!root)
    return NULL;

  if ((CORE_ADDR)key == root->key)
  {
    if (secondary_key < root->secondary_key)
      return rb_tree_find_node_all_keys(root->left, key, secondary_key,
					third_key);
    else if (secondary_key > root->secondary_key)
      return rb_tree_find_node_all_keys(root->right, key, secondary_key,
					third_key);
    else /* (secondary_key == root->secondary_key)  */
    {
      if ((CORE_ADDR)third_key == root->third_key)
	return root;
      else if ((CORE_ADDR)third_key < root->third_key)
	return rb_tree_find_node_all_keys(root->left, key,
					  secondary_key, third_key);
      else
	return rb_tree_find_node_all_keys(root->right, key,
					  secondary_key, third_key);
    }
  }
  else if ((CORE_ADDR)key < root->key)
    return rb_tree_find_node_all_keys(root->left, key, secondary_key,
				      third_key);
  else
    return rb_tree_find_node_all_keys(root->right, key, secondary_key,
				      third_key);
}


/* This function, given a red-black tree (ROOT), a current position in the
 tree (CUR_NODE), a primary key (KEY), and a SECONDARY_KEY,  searches for
 a node in the tree that matches the keys given, removes the node from
 the tree, and returns a copy of the node.  */
struct rb_tree_node *
rb_tree_find_and_remove_node(struct rb_tree_node **root,
			     struct rb_tree_node *cur_node, long long key,
			     int secondary_key)
{
  struct rb_tree_node *result;

  if (!cur_node)
    return NULL;

  if ((CORE_ADDR)key == cur_node->key)
  {
    if (cur_node->left
	&& cur_node->left->key == (CORE_ADDR)key)
      return rb_tree_find_and_remove_node(root, cur_node->left, key,
					  secondary_key);

    result = rb_tree_remove_node(root, cur_node);
    return result;
  }
  else if ((CORE_ADDR)key < cur_node->key)
    return rb_tree_find_and_remove_node(root, cur_node->left, key,
                                        secondary_key);
  else
    return rb_tree_find_and_remove_node(root, cur_node->right, key,
                                        secondary_key);
}

/* Given a red-black tree NODE, return the node in the tree that has the
 smallest "value".  */
struct rb_tree_node *
rb_tree_minimum(struct rb_tree_node *node)
{
  while (node->left)
    node = node->left;
  return  node;
}

/* Given a NODE in a red-black tree, this function returns the
 descendant of that node in the tree that has the smallest "value"
 that is greater than the "value" of NODE.  */
struct rb_tree_node *
rb_tree_successor(struct rb_tree_node *node)
{
  struct rb_tree_node *y;
  if (node->right)
    return rb_tree_minimum (node->right);
  else
  {
    y = node->parent;
    while (y && node == y->right)
    {
      node = y;
      y = node->parent;
    }
  }
  return y;
}

/* This function takes a red-black tree (ROOT) that has had a node
 removed at X, and restores the red-black properties to the tree.
 It uses the algorithm from pate 274 of the Corman et. al. textbook.  */
void
rb_delete_fixup(struct rb_tree_node **root, struct rb_tree_node *x)
{
  struct rb_tree_node *w;

  /* On entering this function, the tree is not correct.  'x' is carrying
   the "blackness" of the node that was deleted as well as its own color.
   If x is red we can just color it black and be done.  But if 'x' is black
   we need to do some re-coloring and rotating to push the extra blackness
   up the tree (once it reaches the root of the tree everything is properly
   balanced again).

   'w' is the sibling in the tree of 'x'.  'w' must be non-NULL, otherwise
   the tree was messed up to begin with.

   For details about the particular cases mentioned below, see the
   algorithm explanation in the book.  */

  while (x != *root
	 && x->color == BLACK)
  {
    if (x == x->parent->left)  /* x LEFT child of its parent.  */
    {
      w = x->parent->right;

      /* Case 1:  w is RED.  Color it black and do a rotation,
       converting this to case 2, 3 or 4.  */

      if (w->color == RED)   /* Case 1 */
      {
	w->color = BLACK;
	x->parent->color = RED;
	left_rotate (root, x->parent);
	w = x->parent->right;
      }

      /* Case 2: Both of w's children are BLACK (where NULL counts
       as BLACK).  In this case, color w red, and push the blackness
       up the tree one node, making what used to be x's parent be
       the new x (and return to top of loop).  */
      gdb_assert(w != NULL);
      if ((!w->left || w->left->color == BLACK)   /* Case 2  */
	  && (!w->right || w->right->color == BLACK))
      {
	w->color = RED;
	x = x->parent;
      }
      else  /* Cases 3 & 4 (w is black, one of its children is red)  */
      {
	/* Case 3: w's right child is black.  */

	if (!w->right || w->right->color == BLACK)  /* Case 3  */
	{
	  if (w->left)
	    w->left->color = BLACK;
	  w->color = RED;
	  right_rotate (root, w);
	  w = x->parent->right;
	}

	/* Case 4: */
	w->color = x->parent->color;
	x->parent->color = BLACK;
	if (w->right)
	  w->right->color = BLACK;
	left_rotate (root, x->parent);
	x = *root;
      }
    }
    else  /* x is the RIGHT child of its parent.  */
    {
      w = x->parent->left;

      /* Case 1:  w is RED.  Color it black and do a rotation,
       converting this to case 2, 3 or 4.  */

      if (w->color == RED)
      {
	w->color = BLACK;
	x->parent->color = RED;
	right_rotate (root, x->parent);
	w = x->parent->left;
      }

      /* Case 2: Both of w's children are BLACK (where NULL counts
       as BLACK).  In this case, color w red, and push the blackness
       up the tree one node, making what used to be x's parent be
       the new x (and return to top of loop).  */
      gdb_assert(w != NULL);
      if ((!w->right || w->right->color == BLACK)
	  && (!w->left || w->left->color == BLACK))
      {
	w->color = RED;
	x = x->parent;
      }
      else /* Cases 3 & 4 (w is black, one of its children is red)  */
      {
	/* Case 3: w's left  child is black: */
	if (!w->left || w->left->color == BLACK)
	{
	  if (w->right)
	    w->right->color = BLACK;
	  w->color = RED;
	  left_rotate (root, w);
	  w = x->parent->left;
	}

	/* Case 4: */
	w->color = x->parent->color;
	x->parent->color = BLACK;
	if (w->left)
	  w->left->color = BLACK;
	right_rotate (root, x->parent);
	x = *root;
      }
    }
  }
  x->color = BLACK;
}

/* Red-Black tree delete node:  Given a tree (ROOT) and a node in the tree
 (NODE), remove the NODE from the TREE, keeping the tree properly balanced
 and colored, and return a copy of the removed node.  This function uses
 the algorithm on page 273 of the Corman, Leiserson and Rivest textbook
 mentioned previously.

 First we make a copy of the node to be deleted, so we can return the
 data from that node.  We need to make a copy rather than returning the
 node because of the way some tree deletions are handled (see the next
 paragraph).

 The basic idea is: If NODE has no children, just remove it.  If
 NODE has one child, splice out NODE (make its parent point to its
 child).  The tricky part is when NODE has two children.  In that
 case we find the successor to NODE in NODE's right subtree (the
 "smallest" node whose "value" is larger than the "value" of node,
 where "smallest" and "value" are determined by the nodes' keys).
 The successor is guaranteed to have ony one child.  Therefore we
 first splice out the successor (make its parent point to its
 child).  Next we *overwrite the keys and data* of NODE with the
 keys and data of its successor node.  The net effect of this is
 that NODE has been replaced by its successor, and NODE is no longer
 in the tree.

 Finally, we may need to re-color or re-balance a portion of the tree.
 */
struct rb_tree_node *
rb_tree_remove_node(struct rb_tree_node **root, struct rb_tree_node *node)
{
  struct rb_tree_node *deleted_node;
  struct rb_tree_node *z = node;
  struct rb_tree_node *x;
  struct rb_tree_node *y;
  struct rb_tree_node *y_parent;
  int x_child_pos = -1;  /* 0 == left child; 1 == right child  */

  if (dwarf2_debug_inlined_stepping)
    gdb_assert (verify_rb_tree (*root));

  /* Make a copy of the node to be "deleted" from the tree.  The copy is what
   will be returned by this function.  */

  deleted_node = (struct rb_tree_node *) xmalloc (sizeof (struct rb_tree_node));
  deleted_node->key = node->key;
  deleted_node->secondary_key = node->secondary_key;
  deleted_node->third_key = node->third_key;
  deleted_node->data = node->data;
  deleted_node->color = node->color;
  deleted_node->left = NULL;
  deleted_node->right = NULL;
  deleted_node->parent = NULL;

  /* Now proceed to 'delete' the node ("z") from the tree.  */

  /* Removing a node with one child from a red-black tree is not too
   difficult, but removing a node with two children IS difficult.
   Therefore if the node to be removed has at most one child, it
   will be removed directly.

   If "z" has TWO children, we will not actually remove node "z"
   from the tree; instead we will find z's successor in the tree
   (which is guaranteed to have at most one child), remove THAT node
   from the tree, and overwrite the keys and data value in z with
   the keys and data value in z's successor.  */

  /* 'y' will point to the node that actually gets removed from the
   tree.  If 'z' has at most one child, 'y' will point to the same
   node as 'z'.  If 'z' has two children, 'y' will point to 'z's
   successor in the tree.  */

  if (!z->left || !z->right)
    y = z;
  else
    y = rb_tree_successor (z);

  /* 'y' is now guaranteed to have at most one child.  Make 'x' point
   to that child.  If y has no children, x will be NULL.  */

  if (y->left)
    x = y->left;
  else
    x = y->right;

  /* Make y's parent be x's parent (it used to be x's grandparent): */
  if (x)
    x->parent = y->parent;

  y_parent = y->parent;

  /* Make 'x' be the child of y's parent that y used to be: */
  if (!y->parent)
    *root = x;
  else if (y == y->parent->left)
  {
    y->parent->left = x;
    x_child_pos = 0;
  }
  else
  {
    y->parent->right = x;
    x_child_pos = 1;
  }

  /* If y is not the same as 'node', then y is the successor to
   'node'; since node has two children and cannot actually be
   removed from the tree, and since y has now been spliced out of
   the tree, overwrite node's keys and data with y's keys and data.
   (This is why we made a copy of node above, to be the return
   value.)  */

  if (y != node)
  {
    node->key = y->key;
    node->secondary_key = y->secondary_key;
    node->third_key = y->third_key;
    node->data = y->data;
  }

  /* If the color of 'y' was RED, then the properties of the red-black
   tree have not been violated by removing it so nothing else needs
   to be done.  But if the color of y was BLACK, then we need to fix
   up the tree, starting at 'x' (which now occupies the position
   where y was removed).  */

  if (y->color == BLACK && x == NULL && y_parent != NULL)
  {
    struct rb_tree_node *w;

    /* Since x is NULL, we can't call rb_delete_fixup directly (it
     assumes a non-NULL x.  Therefore we do the first iteration of
     the while loop from that function here.  At the end of this
     first iteration, x is no longer NULL, so we can call the
     function on the new non-NULL x.  */

    if (x_child_pos == 0)
      w = y_parent->right;
    else
      w = y_parent->left;

    if (!w)
      x = *root;
    else
    {
      if (w->color == RED)
      {
	w->color = BLACK;
	y_parent->color = RED;
	if (x_child_pos == 0)
	{
	  left_rotate (root, y_parent);
	  w = y_parent->right;
	}
	else
	{
	  right_rotate (root, y_parent);
	  w = y_parent->left;
	}
      }

      if ((!w->left || w->left->color == BLACK)
	  && (!w->right || w->right->color == BLACK))
      {
	w->color = RED;
	x = y_parent;
      }
      else if (x_child_pos == 0)
      {
	if (!w->right || w->right->color == BLACK)
	{
	  if (w->left)
	    w->left->color = BLACK;
	  w->color = RED;
	  right_rotate (root, w);
	  w = y_parent->right;
	}

	w->color = y_parent->color;
	y_parent->color = BLACK;
	if (w->right)
	  w->right->color = BLACK;
	left_rotate (root, y_parent);
	x = *root;
      }
      else
      {
	if (!w->left || w->left->color == BLACK)
	{
	  if (w->right)
	    w->right->color = BLACK;
	  w->color = RED;
	  left_rotate (root, w);
	  w = y_parent->left;
	}

	w->color = y_parent->color;
	y_parent->color = BLACK;
	if (w->left)
	  w->left->color = BLACK;
	right_rotate (root, y_parent);
	x = *root;
      }
    }
  }

  if (y->color == BLACK && x)
    rb_delete_fixup (root, x);

  if (dwarf2_debug_inlined_stepping)
    gdb_assert (verify_rb_tree (*root));

  return deleted_node;
}

/* Given a (red-black) tree structure like the one on the left,
 perform a "left-rotation" so that the result is like the one
 on the right (parent, x, and y are individual tree nodes; a, b,
 and c represent sub-trees, possibly null):

 parent                            parent
 |                                |
 x                                y
 / \               ==>>           / \
 a    y                            x   c
 / \                          / \
 b   c                        a   b
 FIXME: fix ASCII art formatting
 */
void
left_rotate(struct rb_tree_node **root, struct rb_tree_node *x)
{
  struct rb_tree_node *y;

  if (!x->right)
    return;

  y = x->right;

  x->right = y->left;
  if (y->left != NULL)
    y->left->parent = x;

  y->parent = x->parent;

  if (x->parent == NULL)
    *root = y;
  else if (x == x->parent->left)
    x->parent->left = y;
  else
    x->parent->right = y;

  y->left = x;
  x->parent = y;
}

/* Given a (red-black) tree structure like the one on the left,
 perform a "right-rotation" so that the result is like the one
 on the right (parent, x, and y are individual tree nodes; a, b,
 and c represent sub-trees, possibly null):

 parent                            parent
 |                                |
 x                                y
 / \               ==>>           / \
 y    c                            a   x
 / \                                   / \
 a   b                                 b   c
 FIXME: fix ASCII art formatting
 */
void
right_rotate(struct rb_tree_node **root, struct rb_tree_node *x)
{
  struct rb_tree_node *y;

  if (!x->left)
    return;

  y = x->left;

  x->left = y->right;
  if (y->right != NULL)
    y->right->parent = x;

  y->parent = x->parent;

  if (x->parent == NULL)
    *root = y;
  else if (x == x->parent->left)
    x->parent->left = y;
  else
    x->parent->right = y;

  y->right = x;
  x->parent = y;
}

/* Basic binary tree insertion, with parent node, and assuming we know the
 NEW_NODE is not already in the tree.  */
void
plain_tree_insert(struct rb_tree_node **root, struct rb_tree_node *new_node)
{
  struct rb_tree_node *tree = *root;

  if (tree == NULL)
    *root = new_node;
  else if (new_node->key < tree->key)
  {
    if (tree->left)
      plain_tree_insert (&tree->left, new_node);
    else
    {
      tree->left = new_node;
      new_node->parent = tree;
    }
  }
  else if (new_node->key > tree->key)
  {
    if (tree->right)
      plain_tree_insert (&tree->right, new_node);
    else
    {
      tree->right = new_node;
      new_node->parent = tree;
    }
  }
  else if (new_node->key == tree->key)
  {
    if (new_node->secondary_key < tree->secondary_key)
    {
      if (tree->left)
	plain_tree_insert (&tree->left, new_node);
      else
      {
	tree->left = new_node;
	new_node->parent = tree;
      }
    }
    else if (new_node->secondary_key > tree->secondary_key)
    {
      if (tree->right)
	plain_tree_insert (&tree->right, new_node);
      else
      {
	tree->right = new_node;
	new_node->parent = tree;
      }
    }
    else if (new_node->secondary_key == tree->secondary_key)
    {
      if (new_node->third_key < tree->third_key)
      {
	if (tree->left)
	  plain_tree_insert (&tree->left, new_node);
	else
	{
	  tree->left = new_node;
	  new_node->parent = tree;
	}
      }
      else /* if (new_node->third_key > tree->third_key) */
      {
	if (tree->right)
	  plain_tree_insert (&tree->right, new_node);
	else
	{
	  tree->right = new_node;
	  new_node->parent = tree;
	}
      }
    }
  }
}

/* Red-Black tree node insert.  Based on algorithm in "Introduction to
 Algorithms", by Corman, Leiserson, and Rivest, Chapter 14.  The
 resulting binary tree is "roughly balanced", i.e. for any node, the height
 of one subtree will never be more than twice the height of the other.
 Every node has a color, either red or black.  The root is always black;
 the color of a node's children are supposed to be different from the
 color of the node.
 */
void
rb_tree_insert(struct rb_tree_node **root, struct rb_tree_node *tree,
	       struct rb_tree_node *new_node)
{
  struct rb_tree_node *y;

  plain_tree_insert(root, new_node);
  new_node->color = RED;
  while ((new_node != *root) && (new_node->parent != NULL)
	 && (new_node->parent->color == RED))
  {
    if (new_node->parent == new_node->parent->parent->left)
    {
      y = new_node->parent->parent->right;
      if (y && y->color == RED)
      {
	new_node->parent->color = BLACK;
	y->color = BLACK;
	new_node->parent->parent->color = RED;
	new_node = new_node->parent->parent;
      }
      else if (new_node == new_node->parent->right)
      {
	new_node = new_node->parent;
	left_rotate (root, new_node);
      }
      else
      {
	new_node->parent->color = BLACK;
	new_node->parent->parent->color = RED;
	right_rotate (root, new_node->parent->parent);
      }
    }
    else
    {
      y = new_node->parent->parent->left;
      if (y && y->color == RED)
      {
	new_node->parent->color = BLACK;
	y->color = BLACK;
	new_node->parent->parent->color = RED;
	new_node = new_node->parent->parent;
      }
      else if (new_node == new_node->parent->left)
      {
	new_node = new_node->parent;
	right_rotate (root, new_node);
      }
      else
      {
	new_node->parent->color = BLACK;
	new_node->parent->parent->color = RED;
	left_rotate (root, new_node->parent->parent);
      }
    }
  }
  (*root)->color = BLACK;
}

/* End repository sub-section 1:  Red-black trees.  */
/* APPLE LOCAL end red-black trees, part 2.  */

/* Begin repository sub-section 2:  Global repositories data structures.
 This section defines the following:
 MAX_OPEN_DBS  (global constant)

 The following two type definitions got moved earlier in this file,
 but are reproduced below:

 enum db_status       (type)
 struct database_info (type)

 enum db_status { DB_UNKNOWN, DB_OPEN, DB_ABBREVS_LOADED, DB_CLOSED };

 struct database_info {
 char *fullname;
 struct abbrev_info *abbrev_table;
 enum db_status current_status;
 struct rb_tree_node *db_types;
 struct objfile_list_node *num_uses;
 struct dwarf2_cu *dummy_cu;
 struct objfile *dummy_objfile;
 sqlite3 *db;
 };


 repositories (global variable, array of struct database_info)
 num_open_dbs (global variable, int)

 find_open_repository    (function)
 lookup_repository_type  (function)
 initialize_repositories (function)
 open_dwarf_repository   (function)
 close_dwarf_repositories (function)
 dwarf2_read_repository_abbrevs (function)
 */

#define MAX_OPEN_DBS 100

struct database_info *repositories = NULL;
int num_open_dbs = 0;

/* Given an open sqlite3 db (probably obtained from an objfile struct), find and
 return the global repository record for that db.  */
struct database_info *
find_open_repository(sqlite3 *db)
{
  int i;

  for (i = 0; i < num_open_dbs; i++)
  {
    if (repositories[i].db == db)
      return &(repositories[i]);
  }

  return NULL;
}

/* Given a repository TYPE_ID number and the DB repository in which
 it's supposed to be defined, return a struct type containing the
 type definition.  */
static void *
lookup_repository_type (int type_id, sqlite3 *db, struct dwarf2_cu *cu,
			int return_die)
{
  struct database_info *repository = NULL;
  struct type *temp_type = NULL;
  struct die_info *type_die = NULL;
  struct rb_tree_node *new_node = NULL;
  struct rb_repository_data *rb_tmp = NULL;

  repository = find_open_repository (db);
  if (repository)
  {
    if (repository->db_types)
    {
      new_node = rb_tree_find_node (repository->db_types, type_id, -1);
      if (new_node)
      {
	rb_tmp = (struct rb_repository_data *) new_node->data;
	temp_type = rb_tmp->type_data;
	type_die = rb_tmp->die_data;
      }
    }

    if (!new_node)
    {
      struct rb_repository_data *tmp_node;

      type_die = db_lookup_type (type_id, db, repository->abbrev_table);
      new_node = (struct rb_tree_node *)
      xmalloc (sizeof(struct rb_tree_node));

      tmp_node = (struct rb_repository_data *) xmalloc (sizeof (struct rb_repository_data));
      tmp_node->die_data = type_die;
      tmp_node->type_data = NULL;
      new_node->key = type_id;
      new_node->data = (void *) tmp_node;
      new_node->left = NULL;
      new_node->right = NULL;
      new_node->parent = NULL;
      new_node->color = UNINIT;
      rb_tree_insert(&repository->db_types,
		     repository->db_types, new_node);
      temp_type = tag_type_to_type(type_die, cu);
      ((struct rb_repository_data *)new_node->data)->type_data = temp_type;
    }
  }
  else
    internal_error(__FILE__, __LINE__,
		   _("Cannot find open repository.\n"));

  if (temp_type && !type_die->type)
    type_die->type = temp_type;

  if (return_die)
    return type_die;
  else
    return temp_type;
}

/* Initialize the global array of repository records: */
static void
initialize_repositories (void)
{
  int i;

  repositories = (struct database_info *) xmalloc(MAX_OPEN_DBS *
						  sizeof (struct database_info));

  for (i = 0; i < MAX_OPEN_DBS; i++)
  {
    repositories[i].fullname = NULL;
    repositories[i].abbrev_table = NULL;
    repositories[i].current_status = DB_UNKNOWN;
    repositories[i].num_uses = NULL;
    repositories[i].db_types = NULL;
    repositories[i].dummy_cu = NULL;
    repositories[i].dummy_objfile = NULL;
    repositories[i].db = NULL;
  }
}

/* Given a directory and filename for a repository (and an objfile that
 contains compilation units that reference the repository), open
 the repository (if not already open), initialize the appropriate objfile
 fields, and update the corresponding global repository record
 appropriately (including incrementing the use-count).  */
int
open_dwarf_repository(char *dirname, char *filename, struct objfile *objfile,
		      struct dwarf2_cu *cu)
{
  int db_status;
  int i;
  char *fullname;
  size_t fullname_len;

  if (!repositories)
    initialize_repositories();

  gdb_assert(dirname != NULL);
  fullname_len = (strlen(dirname) +  strlen(filename) + 4UL);
  fullname = (char *)xmalloc(fullname_len);
  snprintf(fullname, fullname_len, "%s/%s", dirname, filename);

  if (cu->repository)
  {
    if (strcmp (cu->repository_name, fullname) == 0)
      return SQLITE_OK;
    else
      internal_error (__FILE__, __LINE__,
		      _("Multiple repositories found for a single cu\n"));
  }
  else
  {
    for (i = 0; i < num_open_dbs; i++)
    {
      if (strcmp (fullname, repositories[i].fullname) == 0)
      {
	sqlite3 *db = repositories[i].db;
	if (repositories[i].current_status != DB_OPEN
	    && repositories[i].current_status != DB_ABBREVS_LOADED)
	{
	  sqlite3_open (fullname, &(repositories[i].db));
	  cu->repository = repositories[i].db;
	}
	else
	  cu->repository = db;

	if (!repositories[i].dummy_objfile)
	{
	  repositories[i].dummy_objfile = build_dummy_objfile (objfile);
	}
	if (!repositories[i].dummy_cu)
	{
	  repositories[i].dummy_cu = build_dummy_cu (objfile, cu);
	  repositories[i].dummy_cu->objfile =
	    repositories[i].dummy_objfile;
	  repositories[i].dummy_cu->repository = repositories[i].db;
	}
	increment_use_count (&(repositories[i]), objfile);
	objfile->uses_sql_repository = 1;
	cu->repository_name = fullname;
	return SQLITE_OK;
      }
    }
  }

  db_status = sqlite3_open (fullname, &(repositories[num_open_dbs].db));
  cu->repository = repositories[num_open_dbs].db;

  if (db_status != SQLITE_OK)
    db_error ("main", "sqlite3_open failed", cu->repository);

  objfile->uses_sql_repository = 1;
  cu->repository_name = fullname;

  if (num_open_dbs < MAX_OPEN_DBS)
  {
    repositories[num_open_dbs].fullname = fullname;
    repositories[num_open_dbs].current_status = DB_OPEN;
    increment_use_count (&(repositories[num_open_dbs]), objfile);
    repositories[num_open_dbs].dummy_cu = build_dummy_cu (objfile, cu);
    repositories[num_open_dbs].dummy_objfile = build_dummy_objfile (objfile);
    num_open_dbs++;
  }
  else
    internal_error (__FILE__, __LINE__,
		    _("Too many databases open at once.\n"));

  return db_status;
}


/* Given an open sqlite3 DB (repository), find the appropriate
 global repository record, decrement the use-count, and close
 the database if the use-count hits zero.  */
int
close_dwarf_repositories (struct objfile *objfile)
{
  sqlite3 *db;
  int db_status = 0;
  int i;

  for (i = 0; i < num_open_dbs; i++)
  {
    decrement_use_count (&(repositories[i]), objfile);
    if (repositories[i].num_uses == NULL)
    {
      db = repositories[i].db;
      finalize_stmts (db);
      db_status = sqlite3_close (db);
      repositories[i].abbrev_table = NULL;
      repositories[i].current_status = DB_CLOSED;
      repositories[i].db_types = NULL;
      repositories[i].dummy_cu = NULL;
      obstack_free (&repositories[i].dummy_objfile->objfile_obstack, 0);
      repositories[i].dummy_objfile = NULL;
      repositories[i].db = NULL;
    }
  }

  return db_status;
}

/* Given a compilation unit, find the corresponding db and global
 repository record, check to see if dwarf abbreviations table has
 been read in or not, and read it in if it hasn't.  */
void
dwarf2_read_repository_abbrevs (struct dwarf2_cu *cu)
{
  sqlite3 *db;
  struct database_info *repository = NULL;

  db = cu->repository;
  repository = find_open_repository (db);

  if (repository)
  {
    if (!repository->abbrev_table)
      read_in_db_abbrev_table (&(repository->abbrev_table), db);
  }
}

/* End repository sub-section 2: Global repositories data structures.  */

/* Begin repository sub-section 3:  Accessing the sql database & decoding dies.
 This section defines the following:

 SELECT_DIE_STR  (global constant)
 FIND_STRING_STR (global constant)

 struct attr_pair (struct type);

 db_stmt1 (global variable)
 db_stmt2 (global variable)

 get_uleb128             (function)
 read_in_db_abbrev_table (function)
 fill_in_die_info        (function)
 db_lookup_type          (function)
 db_error                (function)
 build_dummy_cu          (function)
 build_dummy_objfile     (function)
 db_read_1_byte          (function)
 db_read_2_bytes         (function)
 db_read_4_bytes         (function)
 db_read_8_bytes         (function)
 db_read_n_bytes         (function)
 db_read_unsigned_leb128 (function)
 db_read_signed_leb128   (function)
 db_read_attribute_value (function)
 follow_db_ref           (function)
 set_repository_cu_language (function)
 get_repository_name     (function)
 finalize_stmts          (function)
 increment_use_count     (function)
 decrement_use_count     (function)
 */

#define SELECT_DIE_STR "SELECT long_canonical FROM debug_info WHERE die_id == ?"
#define FIND_STRING_STR   "SELECT string FROM debug_str WHERE string_id == ?"

sqlite3_stmt *db_stmt1 = NULL;
sqlite3_stmt *db_stmt2 = NULL;

uint32_t
get_uleb128(uint8_t **addr)
{
  uint32_t result = 0;
  int shift = 0;
  const uint8_t *src = *addr;
  uint8_t byte;
  int bytecount = 0;

  while (1)
  {
    bytecount++;
    byte = *src++;
    result |= (byte & 0x7f) << shift;
    shift += 7;
    if ((byte & 0x80) == 0)
      break;
  }

  *addr += bytecount;

  return result;
}

struct attr_pair {
  int attribute;
  int form;
  struct attr_pair *next;
};

/* */
void
read_in_db_abbrev_table(struct abbrev_info **abbrev_table, sqlite3 *db)
{
  struct attr_pair *attr_node;
  struct attr_pair *a_list;
  struct attr_pair *tail;
  int cur_table_size = 100;
  int db_status;
  const char *select_string = "SELECT ALL abbrev_id, attribute_data FROM debug_abbrev";
  const char *pzTail;
  sqlite3_stmt *dbStmt15;
  int abbrev_id;
  int new_size;
  uint8_t *attributes;
  uint8_t *temp_bytes;
  uint8_t *a_ptr;
  int num_attribs;
  int attrib;
  int form;
  int max_id = 0;
  int idx;
  int attributes_len;

  *abbrev_table = ((struct abbrev_info *)
                   xmalloc(cur_table_size * sizeof(struct abbrev_info)));

  db_status = sqlite3_prepare_v2(db, select_string, strlen(select_string),
				 &dbStmt15, &pzTail);
  if (db_status == SQLITE_OK)
  {
    db_status = sqlite3_step(dbStmt15);
    while (db_status == SQLITE_ROW)
    {
      abbrev_id = sqlite3_column_int(dbStmt15, 0);
      attributes_len = sqlite3_column_bytes(dbStmt15, 1);
      temp_bytes = (uint8_t *)sqlite3_column_blob(dbStmt15, 1);
      attributes = (uint8_t *)xmalloc(attributes_len);
      memcpy(attributes, temp_bytes, attributes_len);
      /* Build up attributes list & abbrev_record */
      if (abbrev_id >=  cur_table_size)
      {
	if (abbrev_id > (2 * cur_table_size))
	  new_size = abbrev_id;
	else
	  new_size = cur_table_size;
	*abbrev_table = ((struct abbrev_info *)
			 xrealloc(*abbrev_table,
				  (new_size
				   * sizeof(struct abbrev_info))));
	cur_table_size = new_size;
      }
      if (abbrev_id > max_id)
	max_id = abbrev_id;
      a_ptr = attributes;
      (*abbrev_table)[abbrev_id].number = abbrev_id;
      (*abbrev_table)[abbrev_id].tag = (enum dwarf_tag)get_uleb128(&a_ptr);
      (*abbrev_table)[abbrev_id].has_children = (unsigned short)(*a_ptr);
      (*abbrev_table)[abbrev_id].next = NULL;
      a_ptr++;
      num_attribs = 0;
      a_list = NULL;
      tail = NULL;
      do {
	attrib = get_uleb128(&a_ptr);
	form = get_uleb128(&a_ptr);
	if (form || attrib)
	{
	  num_attribs++;
	  attr_node = ((struct attr_pair *)
		       xmalloc(sizeof(struct attr_pair)));
	  attr_node->attribute = attrib;
	  attr_node->form = form;
	  if (!a_list)
	    a_list = attr_node;
	  if (tail)
	    tail->next = attr_node;
	  tail = attr_node;
	}
      } while ((attrib != 0) || (form != 0));
      (*abbrev_table)[abbrev_id].num_attrs = (unsigned short)num_attribs;
      (*abbrev_table)[abbrev_id].attrs =
	((struct attr_abbrev *)
	 xmalloc(num_attribs * sizeof(struct attr_abbrev)));
      for (attr_node = a_list, idx = 0;
	   attr_node && (idx < num_attribs);
	   attr_node = attr_node->next, idx++)
      {
	if (attr_node->attribute == DW_AT_type)
	{
	  (*abbrev_table)[abbrev_id].attrs[idx].name =
	    DW_AT_APPLE_repository_type;
	}
	else
	{
	  (*abbrev_table)[abbrev_id].attrs[idx].name =
	    (enum dwarf_attribute)attr_node->attribute;
	}
	(*abbrev_table)[abbrev_id].attrs[idx].form =
	  (enum dwarf_form)attr_node->form;
      }
      db_status = sqlite3_step(dbStmt15);
    }
    if ((db_status != SQLITE_OK) && (db_status != SQLITE_DONE))
      db_error("read_in_abbrev_table", "sqlite3_step failed", db);
  }
  else
    db_error("read_in_abbrev_table", "sqlite3_prepare_v2 failed", db);

  db_status = sqlite3_finalize(dbStmt15);
  if (db_status != SQLITE_OK)
    db_error("read_in_abbrev_table", "sqlite3_finalize failed", db);
}

/* */
void
fill_in_die_info(struct die_info *new_die, int die_len, uint8_t *die_bytes,
		 uint8_t *d_ptr, struct abbrev_info *abbrev_table,
		 sqlite3 *db)
{
  int i;
  struct abbrev_info abbrev;
  int abbrev_id = new_die->abbrev;
  int num_attrs = new_die->num_attrs;

  abbrev = abbrev_table[abbrev_id];
  new_die->attrs = ((struct attribute *)
                    xmalloc(num_attrs * sizeof(struct attribute)));
  for (i = 0; i < abbrev.num_attrs; i++)
  {
    new_die->attrs[i].name = abbrev.attrs[i].name;
    db_read_attribute_value(&(new_die->attrs[i]),
			    abbrev.attrs[i].form,
			    &d_ptr);
  }

  if (abbrev.has_children)
  {
    int j;
    int num_children = get_uleb128 (&d_ptr);
    struct die_info *last_child = NULL;

    for (j = 0; ((j < num_children)
		 && (d_ptr < (die_bytes + die_len))); j++)
    {
      uint32_t child_id = get_uleb128(&d_ptr);
      if (child_id == new_die->repository_id)
	internal_error(__FILE__, __LINE__,
		       _("Recursive child id in repository?\n"));
      if (!last_child)
      {
	new_die->child = db_lookup_type(child_id, db, abbrev_table);
	last_child = new_die->child;
	gdb_assert(last_child != NULL);
	last_child->parent = new_die;
      }
      else
      {
	last_child->sibling = db_lookup_type(child_id, db, abbrev_table);
	last_child = last_child->sibling;
	gdb_assert(last_child != NULL);
	last_child->parent = new_die;
      }
    }
  }
}

struct die_info *
db_lookup_type(int type_id, sqlite3 *db, struct abbrev_info *abbrev_table)
{
  int db_status;
  int die_len;
  uint8_t *tmp_bytes;
  uint8_t *die_bytes;
  uint8_t *d_ptr;
  const char *pzTail;
  struct die_info *new_die = NULL;

  db_status = sqlite3_prepare_v2 (db, SELECT_DIE_STR,
				  strlen (SELECT_DIE_STR), &db_stmt1, &pzTail);
  if (db_status == SQLITE_OK)
  {
    db_status = sqlite3_bind_int (db_stmt1, 1, type_id);

    if (db_status != SQLITE_OK)
      db_error ("db_lookup_type", "sqlite3_bind_int failed", db);

    db_status = sqlite3_step (db_stmt1);

    if (db_status == SQLITE_ROW)
    {
      die_len = sqlite3_column_bytes (db_stmt1, 0);
      tmp_bytes = (uint8_t *) sqlite3_column_blob (db_stmt1, 0);
      die_bytes = (uint8_t *) xmalloc (die_len);
      memcpy (die_bytes, tmp_bytes, die_len);
      d_ptr = die_bytes;

      new_die = (struct die_info *) xmalloc (sizeof (struct die_info));

      new_die->abbrev = get_uleb128 (&d_ptr);
      new_die->tag = abbrev_table[new_die->abbrev].tag;
      new_die->offset = 0;
      new_die->repository_id = type_id;
      new_die->next_ref = NULL;
      new_die->type = NULL;
      new_die->child = NULL;
      new_die->sibling = NULL;
      new_die->parent = NULL;
      new_die->num_attrs = abbrev_table[new_die->abbrev].num_attrs;
      fill_in_die_info (new_die, die_len, die_bytes, d_ptr, abbrev_table, db);
    }
    else if (db_status != SQLITE_OK && db_status != SQLITE_DONE)
      db_error ("db_lookup_type", "sqlite3_step failed", db);

    while (db_status == SQLITE_ROW)
      db_status = sqlite3_step (db_stmt1);
  }
  else
    db_error("db_lookup_type",
	     (char *)(db_stmt1
		      ? "sqlite3_reset failed" : "sqlite3_prepare_v2 failed"),
	     db);

  return new_die;
}

/* */
void ATTR_NORETURN
db_error(const char *function_name, const char *db_action_description,
	 sqlite3 *db)
{
  size_t len = (strlen(sqlite3_errmsg(db)) + 1UL);
  char *message = (char *)xmalloc(len);
  strcpy(message, sqlite3_errmsg(db));
  finalize_stmts(db);
  sqlite3_close(db);
  internal_error(__FILE__, __LINE__, "%s", message);
}

/* */
struct dwarf2_cu *
build_dummy_cu(struct objfile *old_objfile, struct dwarf2_cu *old_cu)
{
  struct dwarf2_cu *new_cu;

  new_cu = (struct dwarf2_cu *)xmalloc(sizeof(struct dwarf2_cu));

  memset(new_cu, 0, sizeof(struct dwarf2_cu));
  obstack_init(&old_cu->comp_unit_obstack);
  new_cu->language = old_cu->language;
  if (old_cu->producer)
    new_cu->producer = xstrdup(old_cu->producer);
  /* APPLE LOCAL: Copy the added comp_dir field: */
  if (old_cu->comp_dir)
    new_cu->comp_dir = xstrdup(old_cu->comp_dir);
  new_cu->language_defn = old_cu->language_defn;
  new_cu->repository = old_cu->repository;
  if (old_cu->repository_name)
  {
    new_cu->repository_name = ((char *)
			       xmalloc(strlen(old_cu->repository_name) + 1UL));
    strcpy (new_cu->repository_name, old_cu->repository_name);
  }

  memset(new_cu->ftypes, 0, FT_NUM_MEMBERS * sizeof(struct type *));

  return new_cu;
}

struct objfile *
build_dummy_objfile(struct objfile *old_objfile)
{
  struct objfile *new_objfile;

  new_objfile = (struct objfile *)xmalloc(sizeof(struct objfile));

  memset(new_objfile, 0, sizeof(struct objfile));
  new_objfile->md = NULL;
  obstack_specify_allocation(&new_objfile->objfile_obstack, 0, 0, xmalloc,
			     xfree);

  new_objfile->data = NULL;
  new_objfile->num_data = get_objfile_registry_num_registrations();
  new_objfile->data = XCALLOC(new_objfile->num_data, void *);
  new_objfile->sect_index_text = -1;
  new_objfile->sect_index_data = -1;
  new_objfile->sect_index_bss = -1;
  new_objfile->sect_index_rodata = -1;
  new_objfile->uses_sql_repository = 1;

  return new_objfile;
}

static uint8_t
db_read_1_byte(uint8_t *info_ptr)
{
  uint8_t src = *info_ptr;
  return src;
}

static uint16_t
db_read_2_bytes(uint8_t *info_ptr)
{
  uint16_t src = *((uint16_t *)info_ptr);
  if (byte_swap_p)
    return (uint16_t)(((src & 0x00ff) << 8) | ((src & 0xff00) >> 8));
  else
    return src;
}

static uint32_t
db_read_4_bytes(uint8_t *info_ptr)
{
  uint32_t src = *((uint32_t *)info_ptr);
  if (byte_swap_p)
    return ((uint32_t)
            (((src & 0x000000ff) << 24) |
             ((src & 0x0000ff00) << 8) |
             ((src & 0x00ff0000) >> 8) |
             ((src & 0xff000000) >> 24)));
  else
    return (uint32_t)src;
}

static uint64_t
db_read_8_bytes(uint8_t *info_ptr)
{
  union {
    char c[8];
    uint64_t i;
  } in, out;
  in.i = *((uint64_t *)info_ptr);

  if (byte_swap_p)
  {
    out.c[0] = in.c[7];
    out.c[1] = in.c[6];
    out.c[2] = in.c[5];
    out.c[3] = in.c[4];
    out.c[4] = in.c[3];
    out.c[5] = in.c[2];
    out.c[6] = in.c[1];
    out.c[7] = in.c[0];
  }
  else
    out.i = in.i;
  return out.i;
}

static uint8_t *
db_read_n_bytes(uint8_t *info_ptr, unsigned int num_bytes)
{
  gdb_assert(HOST_CHAR_BIT == 8);
  return info_ptr;
}

static uint32_t
db_read_unsigned_leb128(uint8_t *info_ptr, unsigned int *bytes_read)
{
  uint32_t result = 0U;
  int shift = 0;
  const uint8_t *src = (const uint8_t *)info_ptr;
  uint8_t byte;
  int bytecount = 0;

  while (1)
  {
    bytecount++;
    byte = *src++;
    result |= (byte & 0x7f) << shift;
    shift += 7;
    if ((byte & 0x80) == 0)
      break;
  }

  *bytes_read = bytecount;

  return result;
}

static int32_t
db_read_signed_leb128(uint8_t *info_ptr, unsigned int *bytes_read)
{
  int32_t result = 0;
  int shift = 0;
  size_t size = (sizeof(uint32_t) * 8UL);
  const uint8_t *src = (const uint8_t *)info_ptr;
  uint8_t byte;
  int bytecount = 0;

  while (1)
  {
    bytecount++;
    byte = *src++;
    result |= (byte & 0x7f) << shift;
    shift += 7;
    if ((byte & 0x80) == 0)
      break;
  }

  if (((size_t)shift < size) && (byte & 0x40))
    result |= (0 - (1 << shift));

  *bytes_read = bytecount;

  return result;
}

uint8_t *
db_read_attribute_value(struct attribute *attr, unsigned form,
                        uint8_t **info_ptr)
{
  unsigned int bytes_read;
  struct dwarf_block *blk;

  attr->form = (enum dwarf_form)form;
  switch (form)
  {
    case DW_FORM_block2:
      blk = (struct dwarf_block *)xmalloc(sizeof(struct dwarf_block));
      blk->size = db_read_2_bytes(*info_ptr);
      *info_ptr += 2;
      blk->data = (char *)db_read_n_bytes(*info_ptr, blk->size);
      *info_ptr += blk->size;
      DW_BLOCK(attr) = blk;
      break;
    case DW_FORM_block4:
      blk = (struct dwarf_block *)xmalloc(sizeof(struct dwarf_block));
      blk->size = db_read_4_bytes(*info_ptr);
      *info_ptr += 4;
      blk->data = (char *)db_read_n_bytes(*info_ptr, blk->size);
      *info_ptr += blk->size;
      DW_BLOCK(attr) = blk;
      break;
    case DW_FORM_data2:
      DW_UNSND(attr) = db_read_2_bytes(*info_ptr);
      *info_ptr += 2;
      break;
    case DW_FORM_data4:
      DW_UNSND(attr) = db_read_4_bytes(*info_ptr);
      *info_ptr += 4;
      break;
    case DW_FORM_data8:
      DW_UNSND(attr) = (unsigned long)db_read_8_bytes(*info_ptr);
      *info_ptr += 8;
      break;
    case DW_FORM_block:
      blk = (struct dwarf_block *)xmalloc(sizeof(struct dwarf_block));
      blk->size = db_read_unsigned_leb128(*info_ptr, &bytes_read);
      *info_ptr += bytes_read;
      blk->data = (char *)db_read_n_bytes(*info_ptr, blk->size);
      *info_ptr += blk->size;
      DW_BLOCK(attr) = blk;
      break;
    case DW_FORM_block1:
      blk = (struct dwarf_block *)xmalloc(sizeof(struct dwarf_block));
      blk->size = db_read_1_byte(*info_ptr);
      *info_ptr += 1;
      blk->data = (char *)db_read_n_bytes(*info_ptr, blk->size);
      *info_ptr += blk->size;
      DW_BLOCK(attr) = blk;
      break;
    case DW_FORM_data1:
      DW_UNSND(attr) = db_read_1_byte(*info_ptr);
      *info_ptr += 1;
      break;
    case DW_FORM_flag:
      DW_UNSND(attr) = db_read_1_byte(*info_ptr);
      *info_ptr += 1;
      break;
    case DW_FORM_sdata:
      DW_SND(attr) = db_read_signed_leb128(*info_ptr, &bytes_read);
      *info_ptr += bytes_read;
      break;
    case DW_FORM_APPLE_db_str:
    case DW_FORM_udata:
      DW_UNSND(attr) = db_read_unsigned_leb128(*info_ptr, &bytes_read);
      *info_ptr += bytes_read;
      break;
    case DW_FORM_indirect:
      form = db_read_unsigned_leb128(*info_ptr, &bytes_read);
      *info_ptr += bytes_read;
      *info_ptr = db_read_attribute_value(attr, form, info_ptr);
      break;
    case DW_FORM_flag_present:
      DW_UNSND(attr) = 1;
      break;
    case DW_FORM_ref1:
    case DW_FORM_ref2:
    case DW_FORM_ref4:
    case DW_FORM_ref8:
    case DW_FORM_ref_udata:
    case DW_FORM_addr:
    case DW_FORM_ref_addr:
    case DW_FORM_string:
    case DW_FORM_strp:
    default:
      error(_("Dwarf Error: Cannot handle %s in DWARF reader [in module %s]"),
	    dwarf_form_name(form),
	    "db_read_attribute_value");
  }
  return *info_ptr;
}

struct die_info *
follow_db_ref(struct die_info *die, struct attribute *repository_spec,
	      struct dwarf2_cu *cu)
{
  int db_id;
  int want_die_p = 1;
  sqlite3 *db = cu->repository;
  struct die_info *db_die;

  db_id = DW_UNSND(repository_spec);
  db_die = (struct die_info *)lookup_repository_type(db_id, db, cu, want_die_p);

  return db_die;
}

void
set_repository_cu_language(unsigned int language, struct dwarf2_cu *old_cu)
{
  sqlite3 *db = old_cu->repository;
  struct database_info *repository = NULL;

  if (!db)
    internal_error(__FILE__, __LINE__, _("Missing database.\n"));

  repository = find_open_repository(db);
  if (repository)
    set_cu_language(language, repository->dummy_cu);
}

struct attribute *
get_repository_name(struct attribute *attr, struct dwarf2_cu *cu)
{
  sqlite3 *db = cu->repository;
  int string_id;
  int db_status;
  struct attribute *name_attribute = NULL;
  const char *pzTail;
  char *name = NULL;

  string_id = DW_UNSND(attr);

  if (db)
  {
    db_status = sqlite3_prepare_v2(db, FIND_STRING_STR,
				   strlen(FIND_STRING_STR), &db_stmt2,
				   &pzTail);

    if (db_status != SQLITE_OK)
      db_error("get_repository_name",
	       (char *)(db_stmt2
			? "sqlite3_reset failed"
			: "sqlite3_prepare_v2 failed"), db);

    db_status = sqlite3_bind_int(db_stmt2, 1, string_id);

    if (db_status != SQLITE_OK)
      db_error("get_repository_name", "sqlite3_bind_int failed", db);

    db_status = sqlite3_step(db_stmt2);
    while (db_status == SQLITE_ROW)
    {
      name = (char *)sqlite3_column_text(db_stmt2, 0);
      db_status = sqlite3_step(db_stmt2);
    }

    if (name)
    {
      name_attribute = ((struct attribute *)
			xmalloc(sizeof(struct attribute)));
      name_attribute->name = DW_AT_name;
      name_attribute->form = DW_FORM_string;
      DW_STRING(name_attribute) = (char *)xmalloc(strlen(name) + 1UL);
      strcpy(DW_STRING(name_attribute), name);
    }
  }

  return name_attribute;
}

/* */
int
finalize_stmts(sqlite3 *db)
{
  int db_status;

  db_status = sqlite3_finalize(db_stmt1);

  if (db_status != SQLITE_OK)
    db_error("finalize_stmts", "failed on db_stmt1", db);

  db_status = sqlite3_finalize(db_stmt2);

  if (db_status != SQLITE_OK)
    db_error("finalize_stmts", "failed on db_stmt2", db);

  db_stmt1 = NULL;
  db_stmt2 = NULL;

  return db_status;
}

/* */
void
increment_use_count(struct database_info *repository, struct objfile *ofile)
{
  struct objfile_list_node *current;
  struct objfile_list_node *new_node;

  /* Check to see if ofile is already in the list; if so, return: */
  for (current = repository->num_uses; current; current = current->next)
    if (current->ofile == ofile)
      return;

  /* We ran off the list without finding ofile, so we need to add it to the
   list (at the front).  */

  new_node = (struct objfile_list_node *)xmalloc(sizeof(struct objfile_list_node));
  new_node->ofile = ofile;
  new_node->next = repository->num_uses;
  repository->num_uses = new_node;
}

void
decrement_use_count(struct database_info *repository, struct objfile *ofile)
{
  struct objfile_list_node *current;
  struct objfile_list_node *prev;

  if (repository->num_uses == NULL)
    return;

  for (prev = NULL, current = repository->num_uses; current;
       prev = current, current = current->next)
  {
    if (current->ofile == ofile)
    {
      if (prev)
	prev->next = current->next;
      else
	repository->num_uses = current->next;
      return;
    }
  }
}

/* End repository sub-section 3: Accessing the sql database & decoding dies. */

/* Functions for debugging red-black trees.  */

/* */
int ATTRIBUTE_USED
num_nodes_in_tree(struct rb_tree_node *tree)
{
  int total;

  if (tree == NULL)
    total = 0;
  else
    total = num_nodes_in_tree(tree->left) + num_nodes_in_tree(tree->right) + 1;

  return total;
}

/* */
int
tree_height(struct rb_tree_node *tree)
{
  int left_height;
  int right_height;
  int height;

  if (tree == NULL)
    height = 0;
  else
  {
    left_height = tree_height(tree->left);
    right_height = tree_height(tree->right);
    if (left_height > right_height)
      height = left_height;
    else
      height = right_height;
    if (tree->color == BLACK)
      height++;
  }

  return height;
}

/* */
int
verify_tree_colors(struct rb_tree_node *tree)
{
  int colors_okay;

  if (tree == NULL)
    colors_okay = 1;
  else if  (tree->color == RED)
    colors_okay = ((!tree->left || tree->left->color == BLACK)
		   && (!tree->right || tree->right->color == BLACK)
		   && verify_tree_colors(tree->left)
		   && verify_tree_colors(tree->right));
  else if (tree->color == BLACK)
    colors_okay = (verify_tree_colors(tree->left)
		   && verify_tree_colors(tree->right));
  else
    colors_okay = 0;

  return colors_okay;
}

/* */
int
verify_tree_heights(struct rb_tree_node *tree)
{
  int heights_okay;

  if (tree == NULL)
    heights_okay = 1;
  else
    heights_okay = (tree_height(tree->left) == tree_height(tree->right));

  return heights_okay;
}

/* */
int
verify_rb_tree(struct rb_tree_node *tree)
{
  if (!verify_tree_colors(tree))
  {
    fprintf(stderr, "rb_tree is not colored correctly.\n");
    return 0;
  }

  if (!verify_tree_heights(tree))
  {
    fprintf(stderr, "rb_tree is not properly balanced.\n");
    return 0;
  }

  return 1;
}

/* */
static void
rb_print_node(struct rb_tree_node *tree)
{
  if (tree == NULL)
    fprintf(stdout, "(NULL)\n");
  else
  {
    if (tree->color == RED)
      fprintf(stdout, "(Red");
    else if (tree->color == BLACK)
      fprintf(stdout, "(Black");
    else
      fprintf(stdout, "(Unknown");
    fprintf(stdout, ", 0x%s, %d, 0x%s)\n", paddr_nz(tree->key),
	    tree->secondary_key, paddr_nz(tree->third_key));
  }
}

/* */
void ATTRIBUTE_USED
rb_print_tree(struct rb_tree_node *tree, int indent_level)
{
  char *spaces;

  spaces = (char *)xmalloc(indent_level);
  memset(spaces, ' ', indent_level);

  fprintf(stdout, "%s", spaces);
  rb_print_node(tree);
  if (tree)
  {
    rb_print_tree(tree->left, (indent_level + 3));
    rb_print_tree(tree->right, (indent_level + 3));
  }

  xfree(spaces);
}

/* End functions for debugging red-black trees.  */

/* APPLE LOCAL end dwarf repository  */
