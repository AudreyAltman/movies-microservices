local _M = {}

local function _StrIsEmpty(s)
  return s == nil or s == ''
end

function _M.Upload()
 local ngx = ngx
 local MovieInfoServiceClient = require "movies_MovieInfoService"
 local GenericObjectPool = require "GenericObjectPool"
  -- Read the parameters sent by the end user client
  
  ngx.req.read_body()
        local post = ngx.req.get_post_args()

 -- ngx.say("Inside Nginx Lua script: Processing Get Movie list... Request from: ", post.movie_name)
  local client = GenericObjectPool:connection(MovieInfoServiceClient, "movie-info-service", 9093)  
  local movie_ids = {"MOV00001","MOV00002","MOV00003","MOV00004","MOV00005","MOV00006","MOV00007","MOV00008","MOV00009","MOV00010","MOV00011","MOV00012","MOV00013","MOV00014","MOV00015"};
  local movie_titles = {"Get Smart","The Mask","Ace Ventura","Avengers 1","Avengers 2","Batman Begins","The Mummy","Sherlock Holmes","Joker","Conjuring","Insidious","Sinister","Naruto","Dragon Ball Z","Boss Baby"};
  local movie_links ={"/Videos/Get_Smart.mp4","/Videos/The_Mask.mp4","/Videos/Ace_Ventura.mp4","/Videos/Avengers1.mp4","/Videos/Avengers2.mp4","/Videos/Batman_Begins.mp4","/Videos/The_Mummy.mp4","/Videos/Sherlock_Holmes.mp4","/Videos/Joker.mp4","/Videos/Conjuring.mp4","/Videos/Insidious.mp4","/Videos/Sinister.mp4","/Videos/Naruto.mp4","/Videos/Dragon Ball Z.mp4","/Videos/Boss_Baby.mp4"};

  local status, ret = pcall(client.UploadMovies, client, movie_ids, movie_titles, movie_links)
  GenericObjectPool:returnConnection(client)
  ngx.say("Status: ",status)

  if not status then

        ngx.status = ngx.HTTP_INTERNAL_SERVER_ERROR
        if (ret.message) then
            ngx.header.content_type = "text/plain"
            ngx.say("Failed to upload Movies: " .. ret.message)
            ngx.log(ngx.ERR, "Failed to upload Movies: " .. ret.message)
        else
            ngx.header.content_type = "text/plain"
            ngx.say("Failed to upload Movies: " )
            ngx.log(ngx.ERR, "Failed to upload Movies: " )
        end
        ngx.exit(ngx.HTTP_OK)
    else
	ngx.header.content_type = "text/plain"
	ngx.say(ret)
    	ngx.exit(ngx.HTTP_OK)
    end

end

return _M
