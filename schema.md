```mermaid

graph TD
    %% Main Program Flow
    A[main] --> B[initialize_shell]
    B --> C1[init_env]
    B --> C2[setup_signals]
    
    %% Main Loop
    A --> D[shell_loop]
    D --> E[readline]
    E --> F[handle_input]
    
    %% PARSING PHASE
    subgraph Parsing Phase
        %% Tokenization
        F --> G[tokenizer]
        
        subgraph Sequential Token Processing
            G --> P1[process_quotes]
            P1 --> P2[process_variables]
            P2 --> P3[process_operators]
            P3 --> P4[process_words]
        end
        
        %% Command Building
        P4 --> H[group_tokens_into_commands]
        
        subgraph Command Creation
            H --> H1[Create Command Structure]
            H1 --> H2[Set Command Arguments]
            H2 --> H3[Setup Redirections]
            H3 --> H4[Link Pipeline Commands]
        end
    end
    
    %% EXECUTION PHASE
    subgraph Execution Phase
        H4 --> I[execute_command]
        
        %% Command Type Check
        I --> J{Pipeline?}
        
        %% Pipeline Execution
        J -->|Yes| K[execute_pipeline]
        K --> K1[Setup Pipes]
        K1 --> K2[Fork Processes]
        K2 --> K3[Execute Commands]
        K3 --> K4[Wait & Cleanup]
        
        %% Single Command Execution
        J -->|No| L{Is Builtin?}
        
        %% Builtin Handling
        L -->|Yes| M[execute_builtin]
        M --> M1[echo/cd/pwd]
        M --> M2[export/unset/env]
        M --> M3[exit]
        
        %% External Command
        L -->|No| N[execute_external]
        N --> N1[Fork Process]
        N --> N2[Find Command Path]
        N --> N3[Setup Redirections]
        N --> N4[execve]
    end
    
    %% Return to Loop
    K4 --> R[Update Exit Status]
    M --> R
    N4 --> R
    R --> D
    
    %% Core Data Structures
    subgraph Data Structures
        direction LR
        
        T[Token]
        T --> T1[type]
        T --> T2[value]
        T --> T3[quote_type]
        
        C[Command]
        C --> C1[args]
        C --> C2[redirections]
        C --> C3[pipeline info]
    end
    
    %% Styling
    classDef init fill:#f96,stroke:#333
    classDef loop fill:#69f,stroke:#333
    classDef parse fill:#f9f,stroke:#333
    classDef exec fill:#bfb,stroke:#333
    classDef struct fill:#ddf,stroke:#333
    
    class A,B,C1,C2 init
    class D,E,F loop
    class G,P1,P2,P3,P4,H,H1,H2,H3,H4 parse
    class I,J,K,L,M,N exec
    class T,C struct
    
    %% Subgraph Styling
    style Parsing Phase fill:#fef,stroke:#333
    style Execution Phase fill:#efe,stroke:#333
    style Data Structures fill:#eef,stroke:#333