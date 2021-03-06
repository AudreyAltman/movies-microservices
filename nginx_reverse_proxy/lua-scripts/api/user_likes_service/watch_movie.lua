local _M = {}

local function _StrIsEmpty(s)
  return s == nil or s == ''
end

function _M.Watch_Movie()
  local UserLikesServiceClient = require "movies_UserLikesService"
  local GenericObjectPool = require "GenericObjectPool"
  local ngx = ngx
  -- Read the parameters sent by the end user client

  ngx.req.read_body()
        local post = ngx.req.get_post_args()

        if (_StrIsEmpty(post.user_id) or _StrIsEmpty(post.movie_id)) then
           ngx.status = ngx.HTTP_BAD_REQUEST
           ngx.say("Incomplete arguments")
           ngx.log(ngx.ERR, "Incomplete arguments")
           ngx.exit(ngx.HTTP_BAD_REQUEST)
        end

  ngx.say("Inside Nginx Lua script: Processing User Watch Movie... Request from: ", post.user_id)
  ngx.say("Movie id: ", post.movie_id)

  local client = GenericObjectPool:connection(UserLikesServiceClient, "user-likes-service", 9094)

  local status, ret = pcall(client.UserWatchMovie, client, post.user_id, post.movie_id)
  GenericObjectPool:returnConnection(client)
  ngx.say("Status: ", status)

  if not status then

        ngx.status = ngx.HTTP_INTERNAL_SERVER_ERROR
        if (ret.message) then
            ngx.header.content_type = "text/plain"
            ngx.say("Failed to upload watch log: " .. ret.message)
            ngx.log(ngx.ERR, "Failed to upload watch log: " .. ret.message)
        else
            ngx.header.content_type = "text/plain"
            ngx.say("Failed to upload watch log: " )
            ngx.log(ngx.ERR, "Failed to upload watch log: " )
        end
        ngx.exit(ngx.HTTP_OK)
    else
        ngx.header.content_type = "text/plain"
        ngx.say("Uploaded watch log\n")
        ngx.exit(ngx.HTTP_OK)
    end

end

return _M
