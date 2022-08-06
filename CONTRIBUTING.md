# Contributing

## Commits

Commit subjects should be short and concise. Further information can be added in the descripton part.

Commits should follow the following format:
```
<category>: <subject>

<further details>
```

Write the subject in imperative mood ("Foo: Add bar", not "Foo: Added bar")

Write your commits in proper English, with care and punctuation.

Wrap your commits at 72 characters.

If you have any images that go along with your code, optipng -strip all on them to optimize and strip away useless metadata. This can reduce file size drastically, from a couple of kilobyes to megabytes.

Make sure your commits are rebased on the `master` branch.

## Code styling

The style this project uses is pretty similar to WebKit with some changes.

We have a maximum column width of 80 characters.

All `struct`s defined must use a `typedef`. They must also have a `_t` appended at the end.

Indentation is done using 2 spaces.

All definitions should be broken after ther return type.:

```c
i32
add(i32 a, i32 b)
{
    return a + b;
}
```

Brackets must be put on new lines for function definitions. This allows for easy regex: `^add`
