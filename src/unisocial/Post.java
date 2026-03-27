package unisocial;

import java.time.LocalDateTime;
import java.util.UUID;

/**
 * Immutable message object broadcast by UniSocialHub.
 */
public final class Post {
    private final UUID id;
    private final String content;
    private final LocalDateTime createdAt;

    public Post(String content) {
        this.id = UUID.randomUUID();
        this.content = content;
        this.createdAt = LocalDateTime.now();
    }

    public UUID getId() {
        return id;
    }

    public String getContent() {
        return content;
    }

    public LocalDateTime getCreatedAt() {
        return createdAt;
    }
}
