#!/bin/bash
# setup.sh - Script to install dependencies, compile the project, and create a sample JSON file with 100+ entries.

# 1. Update package list and install required packages.
echo "Updating package list and installing dependencies..."
sudo apt update
sudo apt install -y qtbase5-dev qmake make wget

# 2. Create the Qt project file (chatbot.pro) if it doesn't exist.
if [ ! -f chatbot.pro ]; then
  echo "Creating chatbot.pro file..."
  cat <<EOL > chatbot.pro
QT += widgets network
CONFIG += c++17
SOURCES += main.cpp
EOL
fi

# 3. Compile the project.
echo "Compiling the project..."
qmake chatbot.pro && make

# 4. Create a sample JSON file with over 100 training entries.
echo "Creating sample.json file with training data..."
cat <<'EOL' > sample.json
[
  {"question": "What is your name?", "answer": "My name is ChatBot."},
  {"question": "What is programming?", "answer": "Programming is the process of writing instructions for computers."},
  {"question": "What is a variable in programming?", "answer": "A variable is a storage location with a symbolic name that holds a value."},
  {"question": "What is a function?", "answer": "A function is a block of code designed to perform a specific task."},
  {"question": "What is C++?", "answer": "C++ is a high-performance programming language used for system/software development."},
  {"question": "What is Python?", "answer": "Python is an interpreted, high-level language known for its readability and ease of use."},
  {"question": "What is Java?", "answer": "Java is a popular, versatile, object-oriented programming language."},
  {"question": "What is a loop?", "answer": "A loop repeatedly executes a block of code until a condition is met."},
  {"question": "What is an array?", "answer": "An array is a collection of elements stored in contiguous memory."},
  {"question": "What is object-oriented programming?", "answer": "It is a paradigm based on the concept of objects that contain data and methods."},
  {"question": "How can I improve my programming skills?", "answer": "Practice daily, read code from others, and work on real projects."},
  {"question": "What is software development?", "answer": "It is the process of designing, coding, testing, and maintaining software."},
  {"question": "What are algorithms?", "answer": "Algorithms are step-by-step procedures for solving problems or performing tasks."},
  {"question": "What is debugging?", "answer": "Debugging is the process of identifying and fixing errors in software."},
  {"question": "What is a data structure?", "answer": "A data structure organizes and stores data for efficient access and modification."},
  {"question": "What is a compiler?", "answer": "A compiler translates source code into machine code."},
  {"question": "What is an interpreter?", "answer": "An interpreter executes code directly without prior compilation."},
  {"question": "What is Git?", "answer": "Git is a distributed version control system for tracking code changes."},
  {"question": "What is GitHub?", "answer": "GitHub is a platform for hosting and collaborating on Git repositories."},
  {"question": "What is agile development?", "answer": "Agile development is an iterative approach that emphasizes flexibility and customer feedback."},
  {"question": "What is a sprint in agile?", "answer": "A sprint is a fixed period during which a set amount of work is completed."},
  {"question": "What is lifestyle?", "answer": "Lifestyle refers to the way a person lives, including habits, values, and attitudes."},
  {"question": "How can I improve my lifestyle?", "answer": "Adopt healthy habits such as regular exercise, a balanced diet, and sufficient sleep."},
  {"question": "What is mindfulness?", "answer": "Mindfulness is being fully present and aware of the moment."},
  {"question": "How do I manage stress?", "answer": "Techniques include exercise, meditation, and proper time management."},
  {"question": "What is meditation?", "answer": "Meditation is a practice to focus the mind and achieve mental clarity."},
  {"question": "How do I develop a daily routine?", "answer": "Create a schedule that balances work, exercise, and leisure."},
  {"question": "What is healthy eating?", "answer": "Healthy eating involves a balanced diet rich in fruits, vegetables, and whole grains."},
  {"question": "Why is sleep important?", "answer": "Sleep is essential for physical recovery, mental health, and overall well-being."},
  {"question": "What is work-life balance?", "answer": "It is the equilibrium between professional responsibilities and personal life."},
  {"question": "How do I improve my time management?", "answer": "Prioritize tasks, set goals, and use planning tools like calendars."},
  {"question": "What is writing?", "answer": "Writing is the process of expressing thoughts, ideas, or stories in written form."},
  {"question": "How can I improve my writing skills?", "answer": "Practice regularly, read widely, and seek constructive feedback."},
  {"question": "What is creative writing?", "answer": "Creative writing uses imagination to craft stories, poetry, or other narrative works."},
  {"question": "What is a novel?", "answer": "A novel is a long fictional narrative that explores characters and themes."},
  {"question": "What is poetry?", "answer": "Poetry uses rhythmic and aesthetic language to evoke emotions and ideas."},
  {"question": "How do I overcome writer's block?", "answer": "Try freewriting, changing your environment, or taking a short break."},
  {"question": "What is editing?", "answer": "Editing is the process of reviewing and refining written content for clarity and accuracy."},
  {"question": "What is storytelling?", "answer": "Storytelling is the art of conveying a narrative through words, images, or sounds."},
  {"question": "How do I build a narrative?", "answer": "Develop characters, establish conflict, and structure your plot logically."},
  {"question": "What is web development?", "answer": "Web development involves creating and maintaining websites."},
  {"question": "What is frontend development?", "answer": "Frontend development focuses on the user interface and user experience."},
  {"question": "What is backend development?", "answer": "Backend development deals with server-side logic, databases, and application integration."},
  {"question": "What is full-stack development?", "answer": "Full-stack development covers both frontend and backend aspects."},
  {"question": "What is a database?", "answer": "A database is an organized collection of data that can be easily accessed and managed."},
  {"question": "What is SQL?", "answer": "SQL is a language for managing and querying relational databases."},
  {"question": "What is NoSQL?", "answer": "NoSQL databases are designed for large-scale data storage that does not require a fixed schema."},
  {"question": "What is a REST API?", "answer": "A REST API allows communication between systems using standard HTTP methods."},
  {"question": "What is JSON?", "answer": "JSON is a lightweight data-interchange format that is easy for humans to read and write."},
  {"question": "What is XML?", "answer": "XML is a markup language that defines a set of rules for encoding documents."},
  {"question": "What is machine learning?", "answer": "Machine learning uses algorithms to learn patterns from data."},
  {"question": "What is deep learning?", "answer": "Deep learning is a subset of machine learning that uses neural networks with many layers."},
  {"question": "What is data science?", "answer": "Data science involves extracting insights and knowledge from data using various techniques."},
  {"question": "What is AI?", "answer": "Artificial intelligence is the simulation of human intelligence in machines."},
  {"question": "What is natural language processing?", "answer": "NLP is the field of AI that focuses on the interaction between computers and human language."},
  {"question": "What is big data?", "answer": "Big data refers to extremely large data sets that may be analyzed computationally to reveal patterns."},
  {"question": "What is cloud computing?", "answer": "Cloud computing delivers computing services over the internet on a pay-as-you-go basis."},
  {"question": "What is DevOps?", "answer": "DevOps is a set of practices that combines software development and IT operations."},
  {"question": "What is a microservice?", "answer": "A microservice is a small, independent service that performs a single function in a larger application."},
  {"question": "What is containerization?", "answer": "Containerization packages software in a way that it can run reliably across different computing environments."},
  {"question": "What is Docker?", "answer": "Docker is a platform for developing, shipping, and running applications in containers."},
  {"question": "What is Kubernetes?", "answer": "Kubernetes is an open-source system for automating the deployment and management of containerized applications."},
  {"question": "What is responsive design?", "answer": "Responsive design ensures that web content adapts to different screen sizes and devices."},
  {"question": "What is cybersecurity?", "answer": "Cybersecurity protects systems and networks from digital attacks."},
  {"question": "What is ethical hacking?", "answer": "Ethical hacking is testing systems for vulnerabilities to improve security."},
  {"question": "What is version control?", "answer": "Version control tracks changes to files and allows multiple people to work on them concurrently."},
  {"question": "What is algorithm complexity?", "answer": "Algorithm complexity measures the efficiency of an algorithm in terms of time and space."},
  {"question": "What is recursion?", "answer": "Recursion is a method where a function calls itself to solve a problem."},
  {"question": "What is an IDE?", "answer": "An IDE is an Integrated Development Environment that provides tools to develop software."},
  {"question": "What is continuous integration?", "answer": "Continuous integration is the practice of merging all developer working copies to a shared mainline several times a day."},
  {"question": "What is continuous deployment?", "answer": "Continuous deployment automatically deploys code changes to production after passing tests."},
  {"question": "How do I learn to code?", "answer": "Start with beginner-friendly tutorials and practice by building small projects."},
  {"question": "What is object-oriented design?", "answer": "It is a method of design that organizes software around objects rather than functions."},
  {"question": "What is functional programming?", "answer": "Functional programming treats computation as the evaluation of mathematical functions without changing state."},
  {"question": "How do I become a better programmer?", "answer": "Practice consistently, learn new languages, and review code written by experienced developers."},
  {"question": "What is the internet?", "answer": "The internet is a global network connecting millions of private, public, academic, business, and government networks."},
  {"question": "What is blockchain?", "answer": "Blockchain is a decentralized, distributed ledger that records transactions across many computers."},
  {"question": "What is cryptocurrency?", "answer": "Cryptocurrency is a digital asset that uses cryptography for secure transactions."},
  {"question": "What is a smart contract?", "answer": "A smart contract is a self-executing contract with the terms directly written into code."},
  {"question": "What is virtual reality?", "answer": "Virtual reality is an immersive, computer-generated environment that simulates a realistic experience."},
  {"question": "What is augmented reality?", "answer": "Augmented reality overlays digital content onto the real world."},
  {"question": "What is data mining?", "answer": "Data mining is the process of discovering patterns and knowledge from large data sets."},
  {"question": "What is the role of a data scientist?", "answer": "A data scientist analyzes and interprets complex data to help inform business decisions."},
  {"question": "What is big data analytics?", "answer": "It involves examining large and varied data sets to uncover hidden patterns and insights."},
  {"question": "What is a framework in programming?", "answer": "A framework provides a standard way to build and deploy applications."},
  {"question": "What is a library in programming?", "answer": "A library is a collection of precompiled routines that a program can use."},
  {"question": "What is an API?", "answer": "API stands for Application Programming Interface, which allows software applications to interact with each other."},
  {"question": "What is UI/UX design?", "answer": "UI/UX design focuses on creating user-friendly and aesthetically pleasing interfaces."},
  {"question": "What is digital marketing?", "answer": "Digital marketing is the promotion of products or brands through digital channels."},
  {"question": "What is social media?", "answer": "Social media platforms allow users to create and share content, and to interact with others."},
  {"question": "What is e-commerce?", "answer": "E-commerce refers to buying and selling goods and services online."},
  {"question": "What is SEO?", "answer": "SEO stands for Search Engine Optimization, which improves the visibility of a website."},
  {"question": "What is a podcast?", "answer": "A podcast is a digital audio file available for streaming or download."},
  {"question": "What is online education?", "answer": "Online education delivers learning content via the internet."},
  {"question": "What is augmented reality?", "answer": "Augmented reality integrates digital information with the user’s environment in real time."},
  {"question": "What is a startup?", "answer": "A startup is a company designed for rapid growth, often in innovative fields."},
  {"question": "What is entrepreneurship?", "answer": "Entrepreneurship is the process of starting and running a new business."},
  {"question": "What is a mentor?", "answer": "A mentor is an experienced advisor who helps guide someone’s career or personal growth."},
  {"question": "What is networking?", "answer": "Networking is building relationships with others for professional or personal development."},
  {"question": "What is a career?", "answer": "A career is an occupation or profession that a person engages in for a significant period of their life."},
  {"question": "What is self-improvement?", "answer": "Self-improvement is the process of actively improving oneself through learning and personal growth."},
  {"question": "What is goal setting?", "answer": "Goal setting is identifying what you want to achieve and outlining steps to accomplish it."},
  {"question": "What is time management?", "answer": "Time management involves organizing your time efficiently to achieve your goals."},
  {"question": "How do I stay motivated?", "answer": "Set clear goals, break tasks into smaller steps, and celebrate small victories along the way."}
]
EOL

echo "Setup complete!"
echo "You can run your application with: ./chatbot"
echo "A sample JSON file 'sample.json' with 100+ entries has been created."
