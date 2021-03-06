/*
 * Copyright 2013-2016 Emmanuel Engelhart <kelson@kiwix.org>
 * Copyright 2016 Matthieu Gautier <mgautier@kymeria.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU  General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#ifndef OPENZIM_ZIMWRITERFS_ARTICLE_H
#define OPENZIM_ZIMWRITERFS_ARTICLE_H

#include <zim/blob.h>
#include <zim/writer/creator.h>
#include <string>

extern std::string favicon;

class Article : public zim::writer::Article
{
 protected:
  char ns;
  std::string url;
  std::string title;
  std::string mimeType;
  zim::writer::Url redirectUrl;

 public:
  virtual zim::writer::Url getUrl() const;
  virtual std::string getTitle() const;
  virtual bool isRedirect() const;
  virtual std::string getMimeType() const;
  virtual zim::writer::Url getRedirectUrl() const;
  virtual bool shouldIndex() const;
  virtual bool shouldCompress() const;
  virtual ~Article(){};
};

class MetadataArticle : public zim::writer::Article
{
 protected:
   std::string id;

 public:
  explicit MetadataArticle(const std::string& id) : id(id) {};
  virtual zim::writer::Url getUrl() const { return zim::writer::Url('M', id); }
  virtual zim::writer::Url getRedirectUrl() const { return zim::writer::Url(); }
  virtual bool isInvalid() const { return false; }
  virtual std::string getTitle() const { return ""; }
  virtual bool isRedirect() const { return false; }
  virtual bool isLinktarget() const { return false; }
  virtual bool isDeleted() const { return false; }
  virtual std::string getMimeType() const { return "text/plain"; }
  virtual bool shouldIndex() const { return false; }
  virtual bool shouldCompress() const { return true; }
  virtual std::string getFilename() const { return ""; }
};

class SimpleMetadataArticle : public MetadataArticle
{
 private:
  std::string value;

 public:
  explicit SimpleMetadataArticle(const std::string& id,
                                 const std::string& value);
  virtual zim::Blob getData() const
  {
    return zim::Blob(value.c_str(), value.size());
  }
  virtual zim::size_type getSize() const
  {
    return value.size();
  }
};

class MetadataFaviconArticle : public MetadataArticle
{
 private:
   zim::writer::Url redirectUrl;
 public:
  explicit MetadataFaviconArticle(zim::writer::Url value)
    : MetadataArticle(""), redirectUrl(value) {}
  virtual zim::writer::Url getUrl() const { return zim::writer::Url('-', "favicon"); }
  virtual bool isInvalid() const { return false; }
  virtual std::string getTitle() const { return ""; }
  virtual bool isRedirect() const { return true; }
  virtual std::string getMimeType() const { return "image/png"; }
  virtual zim::writer::Url getRedirectUrl() const { return redirectUrl; }
  virtual bool shouldCompress() const { return false; }
  virtual std::string getFilename() const { return ""; }
  virtual zim::Blob getData() const { return zim::Blob(); }
  virtual zim::size_type getSize() const { return 0; }
};

class MetadataDateArticle : public MetadataArticle
{
 private:
  mutable std::string data;
  void genDate() const;

 public:
  MetadataDateArticle();
  virtual zim::Blob getData() const;
  virtual zim::size_type getSize() const;
};

class FileArticle : public Article
{
 private:
  mutable std::string data;
  mutable bool dataRead;
  bool invalid;
  bool uniqueNs;
  std::string _getFilename() const;
  void readData() const;
  void parseAndAdaptHtml(bool detectRedirects);
  void adaptCss();

 public:
  explicit FileArticle(const std::string& path,
                       bool uniqueNs,
                       const bool detectRedirects = true);
  virtual zim::Blob getData() const;
  virtual bool isLinktarget() const { return false; }
  virtual bool isDeleted() const { return false; }
  virtual zim::size_type getSize() const;
  virtual std::string getFilename() const;
  virtual bool isInvalid() const;
};

class RedirectArticle : public Article
{
 public:
  explicit RedirectArticle(char ns,
                           const std::string& url,
                           const std::string& title,
                           const zim::writer::Url& redirectUrl);
  virtual zim::Blob getData() const { return zim::Blob(); }
  virtual bool isRedirect() const { return true; }
  virtual bool isLinktarget() const { return false; }
  virtual bool isDeleted() const { return false; }
  virtual zim::size_type getSize() const { return 0; }
  virtual std::string getFilename() const  { return ""; }
};

#endif  // OPENZIM_ZIMWRITERFS_ARTICLE_H
