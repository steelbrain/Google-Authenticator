<?hh //strict
// Inspired from: www.idontplaydarts.com https://www.idontplaydarts.com/static/ga.php_.txt
class Google_Authenticator {
  public static ImmMap<string, int> $Characters = ImmMap {
    'A' => 0,  'B' => 1, 'C' => 2,  'D' => 3,
    'E' => 4,  'F' => 5, 'G' => 6,  'H' => 7,
    'I' => 8,  'J' => 9, 'K' => 10,  'L' => 11,
    'M' => 12,  'N' => 13, 'O' => 14,  'P' => 15,
    'Q' => 16,  'R' => 17, 'S' => 18,  'T' => 19,
    'U' => 20,  'V' => 21, 'W' => 22,  'X' => 23,
    'Y' => 24,  'Z' => 25, '2' => 26,  '3' => 27,
    '4' => 28,  '5' => 29, '6' => 30,  '7' => 31
  };

  public string $Secret;
  public string $RawSecret;
  public function __construct(string $Secret) {
    $Secret = strtoupper($Secret);
    if (!preg_match('/^[A-Z2-7]+$/', $Secret)) {
      throw new InvalidArgumentException('Invalid Secret provided');
    }
    $Length = strlen($Secret);
    $this->RawSecret = $Secret;
    $this->Secret = '';

    $A = 0;
    $B = 0;

    for($i = 0; $i < $Length; ++$i) {
      $A = ($A << 5) + static::$Characters[$Secret[$i]];
      if (($B += 5) >= 8) {
        $B -= 8;
        $this->Secret .= chr(($A & (0xFF << $B)) >> $B);
      }
    }
    if (strlen($this->Secret) < 8) {
      throw new Exception('Secret is too short');
    }
  }
  public function generate(?float $Time = null): string {
    if ($Time === null) {
      $Time = floor(microtime(true) / 30);
    }
    $Time = pack('N*', 0) . pack('N*', $Time);
    $Hash = hash_hmac('sha1', $Time, $this->Secret, true);

    $HashOffset = ord($Hash[19]) & 0xf;
    $Hash = (
        ((ord($Hash[$HashOffset+0]) & 0x7f) << 24 ) |
        ((ord($Hash[$HashOffset+1]) & 0xff) << 16 ) |
        ((ord($Hash[$HashOffset+2]) & 0xff) << 8 ) |
        (ord($Hash[$HashOffset+3]) & 0xff)
    ) % pow(10, 6);
    return str_pad($Hash, 6, '0', STR_PAD_LEFT);
  }
  // Note: apt-get install qrencode
  public function generateQRCode(string $Title, int $Size = 5): string {
    return shell_exec('qrencode '.escapeshellarg('otpauth://totp/'.$Title.'?secret='.$this->RawSecret). ' -o - -s '.$Size);
  }
  public function verify(string $Key): bool {
    return $this->generate() === $Key;
  }

  <<__Memoize>>
  public static function getInstance(string $Secret): Google_Authenticator {
    return new Google_Authenticator($Secret);
  }

}
