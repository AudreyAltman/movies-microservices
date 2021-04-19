local _M = {}

local function _StrIsEmpty(s)
  return s == nil or s == ''
end

function _M.MovieLink()
 local ngx = ngx
 local MovieInfoServiceClient = require "movies_MovieInfoService"
 local GenericObjectPool = require "GenericObjectPool"
  -- Read the parameters sent by the end user client
  
  ngx.req.read_body()
        local post = ngx.req.get_post_args()

        if (_StrIsEmpty(post.movie_name) ) then
           ngx.status = ngx.HTTP_BAD_REQUEST
           ngx.say("Incomplete arguments")
           ngx.log(ngx.ERR, "Incomplete arguments")
           ngx.exit(ngx.HTTP_BAD_REQUEST)
        end

--  ngx.say("Inside Nginx Lua script: Processing Get Movie list... Request from: ", post.movie_name)
  local client = GenericObjectPool:connection(MovieInfoServiceClient, "movie-info-service", 9093)  
  local status, ret = pcall(client.GetMovieLink, client, post.movie_name, 123)
  GenericObjectPool:returnConnection(client)
  ngx.say("Status: ",status)

  if not status then

        ngx.status = ngx.HTTP_INTERNAL_SERVER_ERROR
        if (ret.message) then
            ngx.header.content_type = "text/plain"
            ngx.say("Failed to get Movie link: " .. ret.message)
            ngx.log(ngx.ERR, "Failed to get Movie link: " .. ret.message)
        else
            ngx.header.content_type = "text/plain"
            ngx.say("Failed to get Movie link: " )
            ngx.log(ngx.ERR, "Failed to get Movie link: " )
        end
        ngx.exit(ngx.HTTP_OK)
    else
        ngx.header.content_type = "text/plain"
        ngx.say("Use the below link to wath the movie: ",post.movie_name)
	ngx.say("Link: https://videorack.s3.amazonaws.com",ret)
        ngx.exit(ngx.HTTP_OK)
    end

end

return _M
