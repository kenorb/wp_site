$Id: README.TXT,v 1.1.2.3 2009/01/31 12:56:05 indytechcook Exp $

-- Summary --

This module is meant to work with FeedAPI, FeedAPI Node and Feed Element Mapper to give more control over what happens with incomming taxonomy terms.

Example Use.
Vocabulary of 'Categories' feeds the a piece of the primary links using taxonomy_menu.  When a feed-item comes in the options->tags element is mapped to the 'Tags' vocabularies using Feed Element Mapper.  If the term is found then I remove it from the association from the 'Tags' vocabulary and add the association to the 'Categories' vocabulary.  Now the new items are automaticly in the correct place for views.

The feed owner (or other person) would then come match the new tags to an existing category.  If they wish to add a new cateogry then they fill out a request form and the site administrator can create the term and move the nodes accordingly.

Suggested Workflow
1. Feed is refreshed
2. New node is created using the taxonomy setting from Feed Element Mapper.  If using FeedAPI Node Inherit, this is applied here also.
3. All mapped and inherited terms are run through this module.
4. Matched terms are added to more defined categoreis.
5. Unmachted terms are placed in a general vocabulary where the feed administrator can add them to specifc categories later.

-- Notes/Assumptions --

*  If a match is found then the node is no longer associated to the original term (optional
*  Tags vocab is not in a tree structure (this will be important when the delete funcitonilty is implemented)
*  Match is NOT case sensitive.

-- REQUIREMENTS --

*  FeedAPI
*  Taxonomy
*  FeedAPI Node
*  Feed Element Mapper (to match the incomming vocabularies.)

-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.

-- CONFIGURATION --

* Associate 2 Vocabularies with the feed-item content type.  One will be used to store the incoming tags and the other will be used to catorize the terms.  It is preferable that the category vocab be well maintenanced.
* Map the tags vocab to an incomming feild.  Normally optiont->tags.
* Go to admin page at admin/settings/feedapi_taxonomy_compare_settings.
* Select the categroy vocab.
* Select Compare To Synonyms (if you want to compare terms to Synoyms).
* Select Remove Original Tag if found (if you want to remove the relationship between the node and original tag).
    
-- TO DO --

* Add a summary of new terms to user.
* Add option to delete the original term if it is no longer assoicated to any nodes.