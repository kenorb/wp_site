UPDATES:

This version of LM Paypal is the first serious attempt to modernise the LM Paypal
codebase, make it more flexible and reusable, and integrate better with other
contributed modules.

In particular, the module is/will-be updated to provide Actions and Triggers.

See LM Paypal issue http://drupal.org/node/328730 for further discussion.

==============================================================================


OLD README:

This is a set of modules to provide donations, paid role or group
subscriptions and paid node posting via PayPal.

These modules are provided with NO warranty or liability stated or implied.
Use it entirely at your own risk!

lm_paypal now consists of four modules:

* lm_paypal which handles talking with PayPal, receiving, validating
and processing PayPal IPN messages. To use the other modules this module
is required.

* lm_paypal_donations which works with lm_paypal to provide donation
buttons

* lm_paypal_subscriptions which works with lm_paypal to provided paid
role or Organic Group memberships under Drupal

* lm_paypal_paid_adverts which works with lm_paypal and
lm_paypal_subscriptions to allow any node type to be flagged as requiring
payment to be published. Articles created with these flagged node types
are only publically viewable once paid for using an admin configured
paypal subscription

Version 1.20 and above can now also co-exist with other Drupal PayPal IPN
modules, such as ecommerce. (Note that subscriptions created by version
1.19 and below still require the notify_url to be set to lm_paypal
on paypal.com.)

Note: Thanks to an idea from Klaus Brune and nekstrom this version of
lm_paypal should be able to co-exist with other Drupal PayPal modules.

Special note: when using these modules all uid's must be in the range 1-65535
and, if using lm_paypal_paid_adverts, the same range applies to node ids.

Incompatibilities:
It has been reported that the module Bad Behaviour can block incoming
IPN responses. I've not had the time to investigate how to correct this.
For now please disable this module.

(see http://drupal.org/node/137182 and http://drupal.org/node/30501)

lm_paypal
---------

This provides all the basic processing needed to successfully use PayPal IPN.

As you can only have one IPN handler registered on your business account
at PayPal you cannot use this module at the same time as any other IPN
based handler!


lm_paypal_subscriptions
-----------------------

Note:
  This is NOT an access control module. To control what the subscribers
  role allows them to do use a module like Taxonomy Access Control.

This module allows admin's to define paid subscriptions to a role or
Organic Group and users to pay to join those subscriptions via PayPal.
The subscribers gain/loose the extra role or group according to the
subscription details.

The subscription definitions are used to generate subscription buttons
that are processed by PayPal. PayPal then sends a variety of messages
back and these are used to control the subscription. Messages such as
signedup, payment, and cancel or end-of-term.

All defined subscriptions are visible to users via a new menu item. They
can also see the status of any subscriptions they currently have via
the standard 'my account > view'.

Each subscription is also available as a block that can be enabled via the
standard block admin page. Note that these block's do not appear if the user
cannot subscribe (not logged in) or already have that subscription.

If you would like more control over the where subscription buttons appear
you can easily do so with a bit of PHP.

The following PHP snippet shows how to make a subscribe button appear for
the subscription with a subid of 1. Admin's can find the subid via the
'admin > LM PayPal Subscriptions' interface.

Note that nothing will appear if they are either not able to subscribe
(not logged in) or if they have already subscribed:

<?php
if (function_exists('lm_paypal_can_subscribe')) {
  $subid = 1;
  if (lm_paypal_can_subscribe ($subid)) {
    print 'Why not subscribe now? ' . lm_paypal_subscribe($subid,8);
  }
}
?>

lm_paypal_donations
-----------------------

Unlike subscriptions there are no donation definitions.

All donations are generated with PHP snippet like the following:

<?php
if (function_exists('lm_paypal_donate')) {
  // 10 = amount, 'USD' is the currency followed by a description
  print 'We would really like a $10 donation ' .
    lm_paypal_donate(10, 'USD', 'donation to example.com') .'<br/>';
  // The amount is a one element array so an text input with the one value as
  //  default
  print 'Write your own amount to give, we suggest $5' .
    lm_paypal_donate(array(5), 'USD', 'donation to example.com') . '<br/>';
  // The amount is a multi element array so a select will be used. Note if one
  //   of the elements is itself an array that will be the default selection
  // The final two parameters are an alternative button (here we use the default)
  //   and a replacement label before the amount
  print 'We would really like a $10, or more, donation ' .
    lm_paypal_donate(array(5,array(10),15), 'USD', 'donation to example.com', '', 'Donation') .'<br/>';
}
?>

If the button is clicked by a logged in user then when the payment
arrives the amount will be associated with them.

Users can see any donations they have made via the standard 'my account > view'.

lm_paypal_paid_adverts
-----------------------

This module allow users to create Paid Advert pages, which are private
by default. They can then make them publically viewable using a PayPal
based subscription.

* This module requires the lm_paypal and lm_paypal_subscription modules
to be installed, enabled and configured.

Initial Configuration

If you are new to this module you need to:

* Update the site specific PayPal settings via lm_paypal admin. Normally
you only need to provide your PayPal Business Email.

* Create one or more Node Subscriptions using lm_paypal new subscriptions.

* Associate these roles with node types via admin / paid adverts.

From now on when users create nodes of types associated with Node
Subscriptions the contents will be private to the author until paid for.

Once a user creates a new node a tab will appear allowing them to pay
for the node with one of the available Node Subscriptions.

Users can view the status of all nodes they have created requiring a
subscription under "my account".


Ecommerce?
----------
If you are looking for a comprehensive ecommerce solution, this isn't it!

If you are looking for a full ecommerce system then I suggest:
http://drupal.org/project/ubercart or http://drupal.org/project/ecommerce
