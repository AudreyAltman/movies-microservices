/**
 * Autogenerated by Thrift Compiler (0.14.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef MovieInfoService_H
#define MovieInfoService_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include <memory>
#include "movies_types.h"

namespace movies {

#ifdef _MSC_VER
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class MovieInfoServiceIf {
 public:
  virtual ~MovieInfoServiceIf() {}
  virtual void GetMoviesByIds(std::vector<std::string> & _return, const std::vector<std::string> & movie_ids) = 0;
  virtual void GetMoviesByTitle(std::vector<std::string> & _return, const std::string& movie_string, const int64_t user_id) = 0;
  virtual void UploadMovies(std::string& _return, const std::vector<std::string> & movie_ids, const std::vector<std::string> & movie_titles, const std::vector<std::string> & movie_links) = 0;
  virtual void GetMovieLink(std::string& _return, const std::string& movie_name, const int64_t user_id) = 0;
};

class MovieInfoServiceIfFactory {
 public:
  typedef MovieInfoServiceIf Handler;

  virtual ~MovieInfoServiceIfFactory() {}

  virtual MovieInfoServiceIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(MovieInfoServiceIf* /* handler */) = 0;
};

class MovieInfoServiceIfSingletonFactory : virtual public MovieInfoServiceIfFactory {
 public:
  MovieInfoServiceIfSingletonFactory(const ::std::shared_ptr<MovieInfoServiceIf>& iface) : iface_(iface) {}
  virtual ~MovieInfoServiceIfSingletonFactory() {}

  virtual MovieInfoServiceIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(MovieInfoServiceIf* /* handler */) {}

 protected:
  ::std::shared_ptr<MovieInfoServiceIf> iface_;
};

class MovieInfoServiceNull : virtual public MovieInfoServiceIf {
 public:
  virtual ~MovieInfoServiceNull() {}
  void GetMoviesByIds(std::vector<std::string> & /* _return */, const std::vector<std::string> & /* movie_ids */) {
    return;
  }
  void GetMoviesByTitle(std::vector<std::string> & /* _return */, const std::string& /* movie_string */, const int64_t /* user_id */) {
    return;
  }
  void UploadMovies(std::string& /* _return */, const std::vector<std::string> & /* movie_ids */, const std::vector<std::string> & /* movie_titles */, const std::vector<std::string> & /* movie_links */) {
    return;
  }
  void GetMovieLink(std::string& /* _return */, const std::string& /* movie_name */, const int64_t /* user_id */) {
    return;
  }
};

typedef struct _MovieInfoService_GetMoviesByIds_args__isset {
  _MovieInfoService_GetMoviesByIds_args__isset() : movie_ids(false) {}
  bool movie_ids :1;
} _MovieInfoService_GetMoviesByIds_args__isset;

class MovieInfoService_GetMoviesByIds_args {
 public:

  MovieInfoService_GetMoviesByIds_args(const MovieInfoService_GetMoviesByIds_args&);
  MovieInfoService_GetMoviesByIds_args& operator=(const MovieInfoService_GetMoviesByIds_args&);
  MovieInfoService_GetMoviesByIds_args() {
  }

  virtual ~MovieInfoService_GetMoviesByIds_args() noexcept;
  std::vector<std::string>  movie_ids;

  _MovieInfoService_GetMoviesByIds_args__isset __isset;

  void __set_movie_ids(const std::vector<std::string> & val);

  bool operator == (const MovieInfoService_GetMoviesByIds_args & rhs) const
  {
    if (!(movie_ids == rhs.movie_ids))
      return false;
    return true;
  }
  bool operator != (const MovieInfoService_GetMoviesByIds_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MovieInfoService_GetMoviesByIds_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class MovieInfoService_GetMoviesByIds_pargs {
 public:


  virtual ~MovieInfoService_GetMoviesByIds_pargs() noexcept;
  const std::vector<std::string> * movie_ids;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _MovieInfoService_GetMoviesByIds_result__isset {
  _MovieInfoService_GetMoviesByIds_result__isset() : success(false) {}
  bool success :1;
} _MovieInfoService_GetMoviesByIds_result__isset;

class MovieInfoService_GetMoviesByIds_result {
 public:

  MovieInfoService_GetMoviesByIds_result(const MovieInfoService_GetMoviesByIds_result&);
  MovieInfoService_GetMoviesByIds_result& operator=(const MovieInfoService_GetMoviesByIds_result&);
  MovieInfoService_GetMoviesByIds_result() {
  }

  virtual ~MovieInfoService_GetMoviesByIds_result() noexcept;
  std::vector<std::string>  success;

  _MovieInfoService_GetMoviesByIds_result__isset __isset;

  void __set_success(const std::vector<std::string> & val);

  bool operator == (const MovieInfoService_GetMoviesByIds_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const MovieInfoService_GetMoviesByIds_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MovieInfoService_GetMoviesByIds_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _MovieInfoService_GetMoviesByIds_presult__isset {
  _MovieInfoService_GetMoviesByIds_presult__isset() : success(false) {}
  bool success :1;
} _MovieInfoService_GetMoviesByIds_presult__isset;

class MovieInfoService_GetMoviesByIds_presult {
 public:


  virtual ~MovieInfoService_GetMoviesByIds_presult() noexcept;
  std::vector<std::string> * success;

  _MovieInfoService_GetMoviesByIds_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _MovieInfoService_GetMoviesByTitle_args__isset {
  _MovieInfoService_GetMoviesByTitle_args__isset() : movie_string(false), user_id(false) {}
  bool movie_string :1;
  bool user_id :1;
} _MovieInfoService_GetMoviesByTitle_args__isset;

class MovieInfoService_GetMoviesByTitle_args {
 public:

  MovieInfoService_GetMoviesByTitle_args(const MovieInfoService_GetMoviesByTitle_args&);
  MovieInfoService_GetMoviesByTitle_args& operator=(const MovieInfoService_GetMoviesByTitle_args&);
  MovieInfoService_GetMoviesByTitle_args() : movie_string(), user_id(0) {
  }

  virtual ~MovieInfoService_GetMoviesByTitle_args() noexcept;
  std::string movie_string;
  int64_t user_id;

  _MovieInfoService_GetMoviesByTitle_args__isset __isset;

  void __set_movie_string(const std::string& val);

  void __set_user_id(const int64_t val);

  bool operator == (const MovieInfoService_GetMoviesByTitle_args & rhs) const
  {
    if (!(movie_string == rhs.movie_string))
      return false;
    if (!(user_id == rhs.user_id))
      return false;
    return true;
  }
  bool operator != (const MovieInfoService_GetMoviesByTitle_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MovieInfoService_GetMoviesByTitle_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class MovieInfoService_GetMoviesByTitle_pargs {
 public:


  virtual ~MovieInfoService_GetMoviesByTitle_pargs() noexcept;
  const std::string* movie_string;
  const int64_t* user_id;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _MovieInfoService_GetMoviesByTitle_result__isset {
  _MovieInfoService_GetMoviesByTitle_result__isset() : success(false), se(false) {}
  bool success :1;
  bool se :1;
} _MovieInfoService_GetMoviesByTitle_result__isset;

class MovieInfoService_GetMoviesByTitle_result {
 public:

  MovieInfoService_GetMoviesByTitle_result(const MovieInfoService_GetMoviesByTitle_result&);
  MovieInfoService_GetMoviesByTitle_result& operator=(const MovieInfoService_GetMoviesByTitle_result&);
  MovieInfoService_GetMoviesByTitle_result() {
  }

  virtual ~MovieInfoService_GetMoviesByTitle_result() noexcept;
  std::vector<std::string>  success;
  ServiceException se;

  _MovieInfoService_GetMoviesByTitle_result__isset __isset;

  void __set_success(const std::vector<std::string> & val);

  void __set_se(const ServiceException& val);

  bool operator == (const MovieInfoService_GetMoviesByTitle_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    if (!(se == rhs.se))
      return false;
    return true;
  }
  bool operator != (const MovieInfoService_GetMoviesByTitle_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MovieInfoService_GetMoviesByTitle_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _MovieInfoService_GetMoviesByTitle_presult__isset {
  _MovieInfoService_GetMoviesByTitle_presult__isset() : success(false), se(false) {}
  bool success :1;
  bool se :1;
} _MovieInfoService_GetMoviesByTitle_presult__isset;

class MovieInfoService_GetMoviesByTitle_presult {
 public:


  virtual ~MovieInfoService_GetMoviesByTitle_presult() noexcept;
  std::vector<std::string> * success;
  ServiceException se;

  _MovieInfoService_GetMoviesByTitle_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _MovieInfoService_UploadMovies_args__isset {
  _MovieInfoService_UploadMovies_args__isset() : movie_ids(false), movie_titles(false), movie_links(false) {}
  bool movie_ids :1;
  bool movie_titles :1;
  bool movie_links :1;
} _MovieInfoService_UploadMovies_args__isset;

class MovieInfoService_UploadMovies_args {
 public:

  MovieInfoService_UploadMovies_args(const MovieInfoService_UploadMovies_args&);
  MovieInfoService_UploadMovies_args& operator=(const MovieInfoService_UploadMovies_args&);
  MovieInfoService_UploadMovies_args() {
  }

  virtual ~MovieInfoService_UploadMovies_args() noexcept;
  std::vector<std::string>  movie_ids;
  std::vector<std::string>  movie_titles;
  std::vector<std::string>  movie_links;

  _MovieInfoService_UploadMovies_args__isset __isset;

  void __set_movie_ids(const std::vector<std::string> & val);

  void __set_movie_titles(const std::vector<std::string> & val);

  void __set_movie_links(const std::vector<std::string> & val);

  bool operator == (const MovieInfoService_UploadMovies_args & rhs) const
  {
    if (!(movie_ids == rhs.movie_ids))
      return false;
    if (!(movie_titles == rhs.movie_titles))
      return false;
    if (!(movie_links == rhs.movie_links))
      return false;
    return true;
  }
  bool operator != (const MovieInfoService_UploadMovies_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MovieInfoService_UploadMovies_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class MovieInfoService_UploadMovies_pargs {
 public:


  virtual ~MovieInfoService_UploadMovies_pargs() noexcept;
  const std::vector<std::string> * movie_ids;
  const std::vector<std::string> * movie_titles;
  const std::vector<std::string> * movie_links;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _MovieInfoService_UploadMovies_result__isset {
  _MovieInfoService_UploadMovies_result__isset() : success(false), se(false) {}
  bool success :1;
  bool se :1;
} _MovieInfoService_UploadMovies_result__isset;

class MovieInfoService_UploadMovies_result {
 public:

  MovieInfoService_UploadMovies_result(const MovieInfoService_UploadMovies_result&);
  MovieInfoService_UploadMovies_result& operator=(const MovieInfoService_UploadMovies_result&);
  MovieInfoService_UploadMovies_result() : success() {
  }

  virtual ~MovieInfoService_UploadMovies_result() noexcept;
  std::string success;
  ServiceException se;

  _MovieInfoService_UploadMovies_result__isset __isset;

  void __set_success(const std::string& val);

  void __set_se(const ServiceException& val);

  bool operator == (const MovieInfoService_UploadMovies_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    if (!(se == rhs.se))
      return false;
    return true;
  }
  bool operator != (const MovieInfoService_UploadMovies_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MovieInfoService_UploadMovies_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _MovieInfoService_UploadMovies_presult__isset {
  _MovieInfoService_UploadMovies_presult__isset() : success(false), se(false) {}
  bool success :1;
  bool se :1;
} _MovieInfoService_UploadMovies_presult__isset;

class MovieInfoService_UploadMovies_presult {
 public:


  virtual ~MovieInfoService_UploadMovies_presult() noexcept;
  std::string* success;
  ServiceException se;

  _MovieInfoService_UploadMovies_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _MovieInfoService_GetMovieLink_args__isset {
  _MovieInfoService_GetMovieLink_args__isset() : movie_name(false), user_id(false) {}
  bool movie_name :1;
  bool user_id :1;
} _MovieInfoService_GetMovieLink_args__isset;

class MovieInfoService_GetMovieLink_args {
 public:

  MovieInfoService_GetMovieLink_args(const MovieInfoService_GetMovieLink_args&);
  MovieInfoService_GetMovieLink_args& operator=(const MovieInfoService_GetMovieLink_args&);
  MovieInfoService_GetMovieLink_args() : movie_name(), user_id(0) {
  }

  virtual ~MovieInfoService_GetMovieLink_args() noexcept;
  std::string movie_name;
  int64_t user_id;

  _MovieInfoService_GetMovieLink_args__isset __isset;

  void __set_movie_name(const std::string& val);

  void __set_user_id(const int64_t val);

  bool operator == (const MovieInfoService_GetMovieLink_args & rhs) const
  {
    if (!(movie_name == rhs.movie_name))
      return false;
    if (!(user_id == rhs.user_id))
      return false;
    return true;
  }
  bool operator != (const MovieInfoService_GetMovieLink_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MovieInfoService_GetMovieLink_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class MovieInfoService_GetMovieLink_pargs {
 public:


  virtual ~MovieInfoService_GetMovieLink_pargs() noexcept;
  const std::string* movie_name;
  const int64_t* user_id;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _MovieInfoService_GetMovieLink_result__isset {
  _MovieInfoService_GetMovieLink_result__isset() : success(false), se(false) {}
  bool success :1;
  bool se :1;
} _MovieInfoService_GetMovieLink_result__isset;

class MovieInfoService_GetMovieLink_result {
 public:

  MovieInfoService_GetMovieLink_result(const MovieInfoService_GetMovieLink_result&);
  MovieInfoService_GetMovieLink_result& operator=(const MovieInfoService_GetMovieLink_result&);
  MovieInfoService_GetMovieLink_result() : success() {
  }

  virtual ~MovieInfoService_GetMovieLink_result() noexcept;
  std::string success;
  ServiceException se;

  _MovieInfoService_GetMovieLink_result__isset __isset;

  void __set_success(const std::string& val);

  void __set_se(const ServiceException& val);

  bool operator == (const MovieInfoService_GetMovieLink_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    if (!(se == rhs.se))
      return false;
    return true;
  }
  bool operator != (const MovieInfoService_GetMovieLink_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MovieInfoService_GetMovieLink_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _MovieInfoService_GetMovieLink_presult__isset {
  _MovieInfoService_GetMovieLink_presult__isset() : success(false), se(false) {}
  bool success :1;
  bool se :1;
} _MovieInfoService_GetMovieLink_presult__isset;

class MovieInfoService_GetMovieLink_presult {
 public:


  virtual ~MovieInfoService_GetMovieLink_presult() noexcept;
  std::string* success;
  ServiceException se;

  _MovieInfoService_GetMovieLink_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class MovieInfoServiceClient : virtual public MovieInfoServiceIf {
 public:
  MovieInfoServiceClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  MovieInfoServiceClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, std::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(std::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(std::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, std::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void GetMoviesByIds(std::vector<std::string> & _return, const std::vector<std::string> & movie_ids);
  void send_GetMoviesByIds(const std::vector<std::string> & movie_ids);
  void recv_GetMoviesByIds(std::vector<std::string> & _return);
  void GetMoviesByTitle(std::vector<std::string> & _return, const std::string& movie_string, const int64_t user_id);
  void send_GetMoviesByTitle(const std::string& movie_string, const int64_t user_id);
  void recv_GetMoviesByTitle(std::vector<std::string> & _return);
  void UploadMovies(std::string& _return, const std::vector<std::string> & movie_ids, const std::vector<std::string> & movie_titles, const std::vector<std::string> & movie_links);
  void send_UploadMovies(const std::vector<std::string> & movie_ids, const std::vector<std::string> & movie_titles, const std::vector<std::string> & movie_links);
  void recv_UploadMovies(std::string& _return);
  void GetMovieLink(std::string& _return, const std::string& movie_name, const int64_t user_id);
  void send_GetMovieLink(const std::string& movie_name, const int64_t user_id);
  void recv_GetMovieLink(std::string& _return);
 protected:
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class MovieInfoServiceProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  ::std::shared_ptr<MovieInfoServiceIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (MovieInfoServiceProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_GetMoviesByIds(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_GetMoviesByTitle(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_UploadMovies(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_GetMovieLink(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  MovieInfoServiceProcessor(::std::shared_ptr<MovieInfoServiceIf> iface) :
    iface_(iface) {
    processMap_["GetMoviesByIds"] = &MovieInfoServiceProcessor::process_GetMoviesByIds;
    processMap_["GetMoviesByTitle"] = &MovieInfoServiceProcessor::process_GetMoviesByTitle;
    processMap_["UploadMovies"] = &MovieInfoServiceProcessor::process_UploadMovies;
    processMap_["GetMovieLink"] = &MovieInfoServiceProcessor::process_GetMovieLink;
  }

  virtual ~MovieInfoServiceProcessor() {}
};

class MovieInfoServiceProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  MovieInfoServiceProcessorFactory(const ::std::shared_ptr< MovieInfoServiceIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::std::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::std::shared_ptr< MovieInfoServiceIfFactory > handlerFactory_;
};

class MovieInfoServiceMultiface : virtual public MovieInfoServiceIf {
 public:
  MovieInfoServiceMultiface(std::vector<std::shared_ptr<MovieInfoServiceIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~MovieInfoServiceMultiface() {}
 protected:
  std::vector<std::shared_ptr<MovieInfoServiceIf> > ifaces_;
  MovieInfoServiceMultiface() {}
  void add(::std::shared_ptr<MovieInfoServiceIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void GetMoviesByIds(std::vector<std::string> & _return, const std::vector<std::string> & movie_ids) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->GetMoviesByIds(_return, movie_ids);
    }
    ifaces_[i]->GetMoviesByIds(_return, movie_ids);
    return;
  }

  void GetMoviesByTitle(std::vector<std::string> & _return, const std::string& movie_string, const int64_t user_id) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->GetMoviesByTitle(_return, movie_string, user_id);
    }
    ifaces_[i]->GetMoviesByTitle(_return, movie_string, user_id);
    return;
  }

  void UploadMovies(std::string& _return, const std::vector<std::string> & movie_ids, const std::vector<std::string> & movie_titles, const std::vector<std::string> & movie_links) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->UploadMovies(_return, movie_ids, movie_titles, movie_links);
    }
    ifaces_[i]->UploadMovies(_return, movie_ids, movie_titles, movie_links);
    return;
  }

  void GetMovieLink(std::string& _return, const std::string& movie_name, const int64_t user_id) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->GetMovieLink(_return, movie_name, user_id);
    }
    ifaces_[i]->GetMovieLink(_return, movie_name, user_id);
    return;
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class MovieInfoServiceConcurrentClient : virtual public MovieInfoServiceIf {
 public:
  MovieInfoServiceConcurrentClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> prot, std::shared_ptr<::apache::thrift::async::TConcurrentClientSyncInfo> sync) : sync_(sync)
{
    setProtocol(prot);
  }
  MovieInfoServiceConcurrentClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, std::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot, std::shared_ptr<::apache::thrift::async::TConcurrentClientSyncInfo> sync) : sync_(sync)
{
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(std::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(std::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, std::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void GetMoviesByIds(std::vector<std::string> & _return, const std::vector<std::string> & movie_ids);
  int32_t send_GetMoviesByIds(const std::vector<std::string> & movie_ids);
  void recv_GetMoviesByIds(std::vector<std::string> & _return, const int32_t seqid);
  void GetMoviesByTitle(std::vector<std::string> & _return, const std::string& movie_string, const int64_t user_id);
  int32_t send_GetMoviesByTitle(const std::string& movie_string, const int64_t user_id);
  void recv_GetMoviesByTitle(std::vector<std::string> & _return, const int32_t seqid);
  void UploadMovies(std::string& _return, const std::vector<std::string> & movie_ids, const std::vector<std::string> & movie_titles, const std::vector<std::string> & movie_links);
  int32_t send_UploadMovies(const std::vector<std::string> & movie_ids, const std::vector<std::string> & movie_titles, const std::vector<std::string> & movie_links);
  void recv_UploadMovies(std::string& _return, const int32_t seqid);
  void GetMovieLink(std::string& _return, const std::string& movie_name, const int64_t user_id);
  int32_t send_GetMovieLink(const std::string& movie_name, const int64_t user_id);
  void recv_GetMovieLink(std::string& _return, const int32_t seqid);
 protected:
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
  std::shared_ptr<::apache::thrift::async::TConcurrentClientSyncInfo> sync_;
};

#ifdef _MSC_VER
  #pragma warning( pop )
#endif

} // namespace

#endif
