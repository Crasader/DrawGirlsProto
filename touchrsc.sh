cd ${0%/*}
find ./Resources/res_img/ -type d -exec touch {} +
find ./Resources/res_sound/ -type d  -exec touch {} +
