using System.Text.RegularExpressions;

var regexNotMd5 = new Regex("^[0-9a-zA-Z]{32}\\..*", RegexOptions.IgnoreCase | RegexOptions.Compiled);
var regexImage = new Regex("\\.(png|jpg|jpeg|gif|tif|tiff)$", RegexOptions.IgnoreCase | RegexOptions.Compiled);
bool doNotRehash = true;


var files = new DirectoryInfo(@".").EnumerateFiles()
	.Where(f => regexImage.IsMatch(f.Name) && (doNotRehash ? !regexNotMd5.IsMatch(f.Name) : true));

foreach (var file in files)
{
	string hashName = await GetFileHash_MD5Async(file.FullName);
	string newFilePathName = Path.Combine(file.DirectoryName!, hashName + Path.GetExtension(file.Name));
	System.IO.File.Move(file.FullName!, newFilePathName);
}


static async Task<string> GetFileHash_MD5Async(string filePath)
{
	using var md5 = System.Security.Cryptography.MD5.Create();
	using var fs = new FileStream(filePath, FileMode.Open, FileAccess.Read);
	var hash = await md5.ComputeHashAsync(fs);
	fs.Close();
	string hashName = Convert.ToHexString(hash).ToLower();
	return hashName;
}