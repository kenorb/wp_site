$Id: README.txt,v 1.1 2008/12/01 23:34:17 alexb Exp $

CSV Parser for FeedAPI

Usage:

- enable module
- create a new content type
- edit the content type, enable FeedAPI, Parser CSV and FeedAPI Node on it


Features and limitations

- Supports CSV and zipped CSV files
- Works off entire CSV files, if they are getting long, your import process might get very slow. There is no way of stopping and picking up where left off yet.
- Format: string should not use quotation marks as delimiters, column delimiter must be comma (,)
