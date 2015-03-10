<?hh

namespace Titon\Console;

use Titon\Test\Stub\Console\CommandStub;
use Titon\Test\TestCase;
use Titon\Console\InputDefinition\Flag;
use Titon\Console\InputDefinition\Option;
use Titon\Console\InputDefinition\Argument;

/**
 * @property \Titon\Console\Input $input
 */
class InputTest extends TestCase {

    public function setUp() {
        $this->input = new Input();
        $this->input->addCommand(new CommandStub());
    }

    public function testParseFlags(): void {
        /*
         * Check basic flag
         */
        $this->input->setInput(Vector {
            'command',
            '--foo'
        });
        $this->input->addFlag((new Flag('foo'))->alias('f'));
        $this->input->parse();

        $this->assertEquals(1, $this->input->getFlag('foo')->getValue());
        $this->assertEquals(1, $this->input->getFlag('f')->getValue());

        $this->input->setInput(Vector {
            'command',
            '-f'
        });
        $this->input->addFlag((new Flag('foo'))->alias('f'));
        $this->input->parse();

        $this->assertEquals(1, $this->input->getFlag('foo')->getValue());
        $this->assertEquals(1, $this->input->getFlag('f')->getValue());

        /*
         * Check stacked, but different, flags
         */
        $this->input->setInput(Vector {
            'command',
            '-fb'
        });
        $this->input->addFlag((new Flag('foo'))->alias('f'));
        $this->input->addFlag((new Flag('bar'))->alias('b'));
        $this->input->parse();

        $this->assertEquals(1, $this->input->getFlag('foo')->getValue());
        $this->assertEquals(1, $this->input->getFlag('bar')->getValue());

        /*
         * Check stacked flag
         */
        $this->input->SetInput(Vector {
            'command',
            '-vvv'
        });
        $this->input->addFlag((new Flag('v'))->setStackable(true));
        $this->input->parse();

        $this->assertEquals(3, $this->input->getFlag('v')->getValue());

        /*
         * Check inverse flags
         */
        $this->input->setInput(Vector {
            '--no-foo'
        });

        $this->input->addFlag(new Flag('foo'));
        $this->input->parse();

        $this->assertEquals(0, $this->input->getFlag('foo')->getValue());
    }

    public function testParseOptions(): void {
        $this->input->setInput(Vector {
            'command',
            '--name',
            'Alex Phillips'
        });
        $this->input->addOption((new Option('name'))->alias('n'));
        $this->input->addArgument(new Argument('bar', 'Bar!'));
        $this->input->parse();

        $this->assertEquals('Alex Phillips', $this->input->getOption('name')->getValue());
        $this->assertEquals('Alex Phillips', $this->input->getOption('n')->getValue());

        $this->input->setInput(Vector {
            '--name',
            'Alex Phillips',
            'command'
        });
        $this->input->addOption((new Option('name'))->alias('n'));
        $this->input->addArgument(new Argument('bar', 'Bar!'));
        $this->input->parse();

        $this->assertEquals('Alex Phillips', $this->input->getOption('name')->getValue());
        $this->assertEquals('Alex Phillips', $this->input->getOption('n')->getValue());

        $this->input->setInput(Vector {
            'command',
            '-n',
            'Alex Phillips'
        });
        $this->input->addOption((new Option('name'))->alias('n'));
        $this->input->addArgument(new Argument('bar', 'Bar!'));
        $this->input->parse();

        $this->assertEquals('Alex Phillips', $this->input->getOption('name')->getValue());
        $this->assertEquals('Alex Phillips', $this->input->getOption('n')->getValue());

        $this->input->setInput(Vector {
            'command',
            '--name="Alex Phillips"'
        });
        $this->input->addOption((new Option('name'))->alias('n'));
        $this->input->addArgument(new Argument('bar', 'Bar!'));
        $this->input->parse();

        $this->assertEquals('Alex Phillips', $this->input->getOption('name')->getValue());
        $this->assertEquals('Alex Phillips', $this->input->getOption('n')->getValue());
    }

    public function testParseArguments(): void {
        $this->input->setInput(Vector {
            'command',
            'Alex Phillips'
        });
        $this->input->addArgument(new Argument('name'));
        $this->input->getActiveCommand();
        $this->input->parse();

        $this->assertEquals('Alex Phillips', $this->input->getArgument('name')->getValue());
    }

    public function testMixedArguments(): void {
        $this->input->setInput(Vector {
            'command',
            'Alex Phillips',
            '-fb',
            '--baz="woot"'
        });
        $this->input->addFlag((new Flag('bar'))->alias('b'));
        $this->input->addOption(new Option('baz'));
        $this->input->addFlag((new Flag('foo'))->alias('f'));
        $this->input->addArgument(new Argument('name'));

        $this->input->getActiveCommand();
        $this->input->parse();

        $this->assertEquals('command', $this->input->getActiveCommand()->getName());
        $this->assertEquals('Alex Phillips', $this->input->getArgument('name')->getValue());
        $this->assertEquals(1, $this->input->getFlag('foo')->getValue());
        $this->assertEquals(1, $this->input->getFlag('f')->getValue());
        $this->assertEquals(1, $this->input->getFlag('bar')->getValue());
        $this->assertEquals(1, $this->input->getFlag('b')->getValue());
        $this->assertEquals('woot', $this->input->getOption('baz')->getValue());
    }
}