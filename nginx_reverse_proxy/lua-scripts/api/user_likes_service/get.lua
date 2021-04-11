local _M = {}

local function _StrIsEmpty(s)
  return s == nil or s == ''
end

function _M.Get()
  local UserLikesServiceClient = require "movies_UserLikesService"
  local GenericObjectPool = require "GenericObjectPool"
  local ngx = ngx
  -- Read the parameters sent by the end user client

  ngx.req.read_body()
        local post = ngx.req.get_post_args()

        if (_StrIsEmpty(post.user_id) ) then
           ngx.status = ngx.HTTP_BAD_REQUEST
           ngx.say("Incomplete arguments")
           ngx.log(ngx.ERR, "Incomplete arguments")
           ngx.exit(ngx.HTTP_BAD_REQUEST)
        end

  ngx.say("Inside Nginx Lua script: Processing Get Ratings... Request from: ", post.user_id)

  local client = GenericObjectPool:connection(UserLikesServiceClient, "user-likes-service", 9094)
  local status, ret = pcall(client.GetUsersLikedMovies, client, post.user_id)
  GenericObjectPool:returnConnection(client)
  ngx.say("Status: ", status)

  if not status then

        ngx.status = ngx.HTTP_INTERNAL_SERVER_ERROR
        if (ret.message) then
            ngx.header.content_type = "text/plain"
            ngx.say("Failed to get ratings: " .. ret.message)
            ngx.log(ngx.ERR, "Failed to get ratings: " .. ret.message)
        else
            ngx.header.content_type = "text/plain"
            ngx.say("Failed to get ratings: " )
            ngx.log(ngx.ERR, "Failed to get ratings: " )
        end
        ngx.exit(ngx.HTTP_OK)
    else
        ngx.header.content_type = "text/plain"
        if (table.getn(ret) > 0) then
            -- Format the returned list of movies so that each appears on a new line
            local str = ""
            for k, v in ipairs(ret) do
                str = str .. v .. "\n"
            end
            ngx.say("Ratings:\n", str)
        else
            ngx.say("There are no ratings for this user")
        end
        ngx.exit(ngx.HTTP_OK)
    end

end

return _M
