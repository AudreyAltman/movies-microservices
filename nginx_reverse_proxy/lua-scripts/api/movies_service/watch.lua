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

        if (_StrIsEmpty(post.movie_name) or _StrIsEmpty(post.user_id)) then
           ngx.status = ngx.HTTP_BAD_REQUEST
           ngx.say("Incomplete arguments")
           ngx.log(ngx.ERR, "Incomplete arguments")
           ngx.exit(ngx.HTTP_BAD_REQUEST)
        end

  ngx.say("Request from user id: ", post.user_id)
  ngx.say("Fetching link for the movie name : ", post.movie_name)
  ngx.say("--------------------------------------------------------")
  local client = GenericObjectPool:connection(MovieInfoServiceClient, "movie-info-service", 9093)  
  local status, ret = pcall(client.GetMovieLink, client, post.movie_name, post.user_id)
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
        ngx.say("Click on the below link to watch the movie: ",post.movie_name)
	ngx.say("Link: https://videorack.s3.amazonaws.com",ret)
	ngx.say(" ")
	ngx.say(" ")
        ngx.exit(ngx.HTTP_OK)
    end

end

return _M
