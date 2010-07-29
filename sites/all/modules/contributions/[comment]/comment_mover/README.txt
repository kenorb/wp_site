This module enables you to move comments and nodes around.

Comments can be

a) moved below another comment on the same node
b) moved to another node
c) converted to a node (eg story, page)

Nodes can be
d) converted to comments

In cases b and c the original comment will contain a link pointing to the place
where the comment is now.

In case d a redirect leads the user to the new location.

In all cases child comments will be moved together with the parent comment or 
node.

The module works with Drupal 5.

Configuration
============= 
1 - After enabling the module, go to administer >> blocks (admin/block) and 
    enable the block "Comment mover clipboard".
2 - Set access permissions:
    - the 'administer comments' permission to move comments and nodes
    - the 'create [nodetype]' permission to convert a comment to [nodetype]
    - the 'update [nodetype]' permission to prune a node
3 - Visit administer >> settings >> comment_mover to configure the module
    - Enable pruning of the following node types
    - Enable pruning of comments on the following node types
    - Enable promotion to the following node types
    - Enable grafting on the following node types

Because not all actions make sense for certain node types (for example
converting comments to polls or vice versa), comment_mover allows you a great
deal of control over the node types it should act on.

Usage
=============
Basic use is simple; it starts by pruning a node or comment via the link 
'prune'. The pruned node/comment now appears in the 'Graft' block. The graft
block shows the title and author of the pruned content and always allows you
to cancel the action. You can now navigate through your site and choose one of
two options: grafting or conversion.

Grafting is a simple matter: simply click the 'graft' link on the content
(comment or node) you want the pruned object to reside under. All child
comments will be moved together with the pruned node or comment.

If you just pruned a comment and enabled story and page under "Enable promotion
to the following node types" (admin/settings/comment_mover), the graft block
also shows you story and page in the selection of node types to which the
comment can be converted. When you then, for example, select 'story' and click
'convert', a story submission form will appear with prefilled fields. Edit if
necessary, then click submit. A new story node will be created. All children of
the original comment will be moved to the new node.

When you move a comment to another node or convert it to a node, the original
comment will contain a link pointing to the comments new location. When you
move a node to become a comment on another node, the original node will contain
a redirect to the new location.

Author
=============
Gerhard Killesreiter

Update to 4.7 and much improved interface:

Heine Deelstra