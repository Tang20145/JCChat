backup_file="backup_$(date +"%Y-%m-%d_%H%M").tar.gz"
backup_dir="./src"
tar -czvf backup/${backup_file} $backup_dir
echo "back up finished:${backup_file}"
