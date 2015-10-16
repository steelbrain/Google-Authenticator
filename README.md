Google-Authenticator
==================

Google-Authenticator is a super-simple Google Authenticator client/engine in Hack/PHP. It has a super-simple API and allows you to even generate QR codes for your ease.

#### API

```hack
class Google_Authenticator {
  public static function getInstance(string $Secret): this
  public function generate(?float $Time = null): string
  public function generateQRCode(string $Title, int $Size = 5): string
  public function verify(string $Key): bool
}
```

#### Examples

```hack
$Auth = Google_Authenticator::getInstance('SomeSecretUserSpecificKey');
$Key = $Auth->generate();
$Valid = $Auth->verify($Key);
echo "Key: $Key\n";
echo "Valid: $Valid\n";
```
Generating a QR code image is super simple too!
```hack
$Auth = Google_Authenticator::getInstance('SomeSecretUserSpecificKey');
header('Content-Type: image/png');
echo $Auth->generateQRCode();
```

#### LICENSE

This project is licensed under the terms of MIT License
