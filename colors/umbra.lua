for k in pairs(package.loaded) do
    if k:match(".*umbra.*") then package.loaded[k] = nil end
end

require('umbra').setup()
require('umbra').colorscheme()
