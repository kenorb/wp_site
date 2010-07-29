// $Id: README.txt,v 1.3 2008/03/30 17:51:46 agentken Exp $

/**
 * @file
 * README file for Domain Source.
 */

Domain Source
Creates a source domain for linking to content from other domains.

CONTENTS
--------

1.  Introduction
1.1  Sponsos
2.  Installation
2.1   Dependencies
2.2   Warning
3.  Module Behavior
3.1   Inherited Permissions
3.2   Content Editing Forms
3.3   Integration with Domain Content
4.  Developer Notes
4.1   Database Schema

----
1.  Introduction

The Domain Source module is a small module that extends Domain Access
by allowing site and affilaite editors to select a primary "source" domain for
all content.

When links are written to content from another domain using the SEO strict
rules, cross-domain sesarching or the "Special Page Requests" rules, the links
will go to the "source" domain specified for the node.

----
1.1  Sponsors

The Domain Source module is sponsored by Dzone, the developer's network.

  http://www.dzone.com

----
2. Installation

This section is not finished.  It requires http://drupal.org/node/210248.

----
2.1 Dependencies

Domain Source requires the Domain Access module be installed and active.

----
2.2 Warning

When this module is enabled, existing content is not automatically assigned
to a source domain.

You must either edit the nodes individually or use the Domain Content
module to perform batch edits.

----
3.  Module Behaviors

The Domain Source module alters the core Domain Access module in a few
subtle ways.  It is only used in the following instances:

  -- You wish to allow searching of all domains from any domain.
  -- You use a content aggregation module such as MySite.
  -- You get "access denied" errors when linking to items on a
      user's Track page.
  -- You want to turn on the advanced setting "Search Engine
      Optimization" to avoid content from being indexed on multiple
      domains.

The default Domain Access behavior in these instances is to rewrite links
to point to the first accessible domain.  With the Domain Source module,
editors with the proper permissions can specify which domain should be
considered "authoritative" for a specific piece of content.

----
3.1 Inherited Permissions

The Domain Source module uses the following permissions from the Domain
Access module:

  -- 'set domain access'
  This permission allows an editor to assign any content to any registered
  domain.  With Domain Source, it also allows users to assign that content to
  any domain as the "source" domain.

  -- 'view domain publishing'
  This permission lets affiliate editors select publishing options for the
  domains on which they are editors.  If these users are allowed to select
  the affiliate domains for their content, they will also be allowed to assign
  the "source" domain from the list of their editable domains.

Note that in all cases, the "source" domain must be selected as a publishing
option.  Failure to do so will return an error when the edit form is submitted.

All other users will have the currently active domain set as the "source"
domain.

----
3.2 Content Editing Forms

Users with the 'view domain publishing' permission will only be able to select
a "source" domain only if the "Content editing forms" setting on Admin > Build
> Domains is set to:

  -- Show user their publishing options
  The node editing form is shown normally, and the user is presented a
  list of checkboxes.  These options represent the affilaite domains that
  the user is allowed to publish content to, according to the domains
  assigned to their user account.

Otherwise, domain assignments will be done automatically, based on the currently
active domain.

----
3.3 Integration with Domain Content

This module adds an element to the Domain Content batch editing screen to
allow for the batch assignment of source domains.

However, this feature is currently only available to users with the 'set domain
access' permission.

----
4.  Developer Notes

The Domain Source module is recommended for most uses.  It is an extension
module because it alters the original design of the Domain Access module.

It is possible that Domain Source will be incorporated into the main module in
later releases.

----
4.1  Database Schema

Installing the module creates a {domain_source} table that contains:

  - nid
  Integer, unique, primary key
  The node id for this record, foreign key to the {node} tabl.

  - domain_id
  Integer, unique
  The lookup key for this record, foreign key to the {domain} table.
