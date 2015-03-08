<?hh // strict
/**
 * @copyright   2010-2015, The Titon Project
 * @license     http://opensource.org/licenses/bsd-license.php
 * @link        http://titon.io
 */

namespace Titon\Console\UserInput;

use Titon\Console\Output;

/**
 * The `Menu` class presents the user with a prompt and a list of available
 * options to choose from.
 *
 * @package Titon\Console\UserInput
 */
class Menu extends AbstractUserInput {

    /**
     * The message to present at the prompt.
     *
     * @var string
     */
    protected string $message;

    /**
     * Construct a new `Menu` with the available choices.
     *
     * @param Map<mixed, mixed> $choices    Available choices accepted
     */
    public function __construct(Map<mixed, mixed> $choices, string $message = '') {
        parent::__construct();

        $this->acceptedValues = $choices;
        $this->message = $message;
    }

    /**
     * {@inheritdoc}
     */
    public function prompt(string $prompt): mixed {
        $keys = $this->acceptedValues->keys();
        $values = $this->acceptedValues->values();

        if ($this->message !== '') {
            $this->output->out($this->message);
        }
        foreach ($values as $index => $item) {
            $this->output->out(sprintf('  %d. %s', $index + 1, (string)$item));
        }

        $this->output->out();

        while (true) {
            $this->output->out("$prompt: ", Output::VERBOSITY_NORMAL, 0);
            $input = $this->input->getUserInput();

            if (is_numeric($input)) {
                $input = (int)$input;
                $input--;
                if (!is_null($values[$input])) {
                    return $keys[$input];
                }

                if ($input < 0 || $input >= $values->count()) {
                    $this->output->error('Invalid menu selection: out of range');
                }
            }
        }
    }
}