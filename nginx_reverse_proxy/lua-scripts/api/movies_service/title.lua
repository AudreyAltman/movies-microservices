local _M = {}

local function _StrIsEmpty(s)
  return s == nil or s == ''
end

function _M.GetTitle()
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

  ngx.say("Request from user_id: ", post.user_id)
  ngx.say("Getting list of all movies matching the pattern: ", post.movie_name)
  ngx.say("--------------------------------------------------------------------")
  local client = GenericObjectPool:connection(MovieInfoServiceClient, "movie-info-service", 9093)  
  local status, ret = pcall(client.GetMoviesByTitle, client, post.movie_name, post.user_id)
  GenericObjectPool:returnConnection(client)
  ngx.say("Status: ",status)

  if not status then

        ngx.status = ngx.HTTP_INTERNAL_SERVER_ERROR
        if (ret.message) then
            ngx.header.content_type = "text/plain"
            ngx.say("Failed to get Movie names: " .. ret.message)
            ngx.log(ngx.ERR, "Failed to get Movie names: " .. ret.message)
        else
            ngx.header.content_type = "text/plain"
            ngx.say("Failed to get Movie names: " )
            ngx.log(ngx.ERR, "Failed to get Movie names: " )
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
            ngx.say("Movie names found:\n", str)
        else
            ngx.say("There are no Movie names found",ret)
        end
        ngx.exit(ngx.HTTP_OK)
    end

end

return _M
